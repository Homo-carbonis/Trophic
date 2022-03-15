// Copyright Ptolom 2022
// This file is part of Trophic.
//
// Trophic is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with this
// program. If not, see <https://www.gnu.org/licenses/>. 

#include<glm/glm.hpp>
#include "metafunction.hpp"
#include<GL/gl.h>

template<typename T>
constexpr GLenum GLType;
template<>
constexpr GLenum GLType<float> = GL_FLOAT;
template<>
constexpr GLenum GLType<double> = GL_DOUBLE;
template<>
constexpr GLenum GLType<int> = GL_INT;
template<>
constexpr GLenum GLType<unsigned int> = GL_UNSIGNED_INT;
template<>
constexpr GLenum GLType<GLboolean> = GL_BOOL;

template<GLenum format>
void attribFormat(GLuint vao, GLuint attribIndex, GLint size, GLenum type,
    GLboolean normalized, GLuint offset);
  template<>
void attribFormat<GL_INT>(GLuint vao, GLuint attribIndex, GLint size, GLenum type,
    GLboolean normalized, GLuint offset) 
{
  glVertexArrayAttribFormat(vao, attribIndex, size, type, normalized, offset);
}
  template<>
void attribFormat<GL_FLOAT>(GLuint vao, GLuint attribIndex, GLint size, GLenum type,
    GLboolean normalized, GLuint offset)
{
  glVertexArrayAttribFormat(vao, attribIndex, size, type, normalized, offset);
}
  template<>
void attribFormat<GL_DOUBLE>(GLuint vao, GLuint attribIndex, GLint size, GLenum type,
    GLboolean normalized, GLuint offset)
{
  glVertexArrayAttribLFormat(vao, attribIndex, size, type, offset);
}

template<typename Type, GLenum format, GLboolean normalized=false>
struct attribute {
  using value=Type;
};

template<typename... Attrs>
struct attributes {
  using types = meta::concat<Attrs...>::value;

  attributes(GLuint vao) : value(vao) {}

  private:
  template<GLuint attribIndex, GLuint offset, typename... As>
    struct attributes_ { attributes_(GLuint vao) {} };

  template<GLuint attribIndex, GLuint offset,
    glm::length_t L, typename T, glm::qualifier Q, GLenum format, GLboolean normalized, typename... As>
      struct attributes_<attribIndex, offset,
    attribute<glm::vec<L,T,Q>, format, normalized>, As...> {
      attributes_(GLuint vao) : a(vao)
      {
        glEnableVertexArrayAttrib(vao, attribIndex);
        attribFormat<format>(vao, attribIndex, L, GLType<T>, normalized, offset);
        glVertexArrayAttribBinding(vao,attribIndex,0);
      }
      attributes_<attribIndex+1, offset+sizeof(glm::vec<L,T,Q>), As...> a;
    };

  template<GLuint attribIndex, GLuint offset,
    glm::length_t R, typename T, glm::qualifier Q,
    GLenum format, GLboolean normalized, typename... As>
      struct attributes_<attribIndex,offset,
    attribute<glm::mat<1,R,T,Q>, format, normalized>, As...> {
      attributes_(GLuint vao) : a(vao) {}
      using Vec=attribute<glm::vec<R,T,Q>, format, normalized>;
      attributes_<attribIndex, offset, Vec, As...> a;
    };
  template<GLuint attribIndex, GLuint offset,
    glm::length_t R, typename T, glm::qualifier Q,
    GLenum format, GLboolean normalized, typename... As>
      struct attributes_<attribIndex,offset,
    attribute<glm::mat<2,R,T,Q>, format, normalized>, As...> {
      attributes_(GLuint vao) : a(vao) {}
      using Vec=attribute<glm::vec<R,T,Q>, format, normalized>;
      attributes_<attribIndex, offset, Vec, Vec, As...> a;
    };
  template<GLuint attribIndex, GLuint offset,
    glm::length_t R, typename T, glm::qualifier Q,
    GLenum format, GLboolean normalized, typename... As>
      struct attributes_<attribIndex,offset,
    attribute<glm::mat<3,R,T,Q>, format, normalized>, As...> {
      attributes_(GLuint vao) : a(vao) {}
      using Vec=attribute<glm::vec<R,T,Q>, format, normalized>;
      attributes_<attribIndex, offset, Vec, Vec, Vec, As...> a;
    };
  template<GLuint attribIndex, GLuint offset,
    glm::length_t R, typename T, glm::qualifier Q,
    GLenum format, GLboolean normalized, typename... As>
      struct attributes_<attribIndex,offset,
    attribute<glm::mat<4,R,T,Q>, format, normalized>, As...> {
      attributes_(GLuint vao) : a(vao) {}
      using Vec=attribute<glm::vec<R,T,Q>, format>;
      attributes_<attribIndex, offset, Vec, Vec, Vec, Vec, As...> a;
    };
  attributes_<0,0,Attrs...> value;
};
