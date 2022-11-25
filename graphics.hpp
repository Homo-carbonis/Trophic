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

#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "VertexSpecification.hpp"
#include "raii.hpp"
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <functional>
#include <png++/png.hpp>
#include <png++/solid_pixel_buffer.hpp>
#include <cstdint>
#include <concepts>
#include "utility.hpp"
#include "SLTuple.hpp"
using namespace std;

class GLObject : public Raii {
  using Raii::Raii;
  public:
  GLObject() : Raii(make_any<GLuint>(0)) {}
  protected:
  GLuint object() const { return any_cast<GLuint>(object_); }
  GLuint* objectPtr() { return any_cast<GLuint>(&object_); }
};

template<typename ShaderType>
ShaderType loadShader(const char* filename) {
  ifstream file(filename);
  char* source = readAll(file);
  file.close();
  cerr << "Compiling shader: "; cerr <<  filename; cerr << "\n";
  ShaderType shader(source);
  delete source;
  return shader;
}

class Shader : public GLObject {
    friend class Program;
    using GLObject::GLObject;
  public:
    Shader(const char* source, const GLenum type)
      : GLObject(glCreateShader(type))
    {
      glShaderSource(object(), 1, &source, NULL);
      glCompileShader(object());
      if (!status())
        throw;
    }
    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;
    bool status() 
    {
      GLint status;
      glGetShaderiv(object(), GL_COMPILE_STATUS, &status);
      return (bool)status;
    }
    string log()
    {
      GLint logLength;
      glGetShaderiv(object(),GL_INFO_LOG_LENGTH,&logLength);
      string log;
      if (logLength) {
        char* logc = new char[logLength];
        glGetShaderInfoLog(object(), 0xffff, NULL, logc);
        log=logc;
        delete logc;
      }
      return log;
    }
    virtual ~Shader() { if (hasObject()) glDeleteShader(object()); }
};

class VertexShader : public Shader {
  public:
    VertexShader(const char* source)
      :Shader(source, GL_VERTEX_SHADER) {}
};
class GeometryShader : public Shader {
  public:
    GeometryShader(const char* source)
      :Shader(source, GL_GEOMETRY_SHADER) {}
};
class FragmentShader : public Shader {
  public:
    FragmentShader(const char* source)
      :Shader(source, GL_FRAGMENT_SHADER) {}
};

class Program : GLObject {
  public:
    using GLObject::GLObject;
    Program(initializer_list<Shader> shaders)
      : GLObject(glCreateProgram())
    {
      for (const Shader& s : shaders) 
        glAttachShader(object(), s.object());
      glLinkProgram(object());
      for (const Shader& s : shaders) 
        glDetachShader(object(), s.object());
      if (!status())
        throw;
      if (!validate())
        throw;
      glUseProgram(object());
    }
    bool status() 
    {
      GLint status;
      glGetProgramiv(object(), GL_LINK_STATUS, &status);
      return (bool)status;
    }
    string log()
    {
      GLint logLength;
      glGetProgramiv(object(),GL_INFO_LOG_LENGTH,&logLength);
      string log;
      if (logLength) {
        char* logc = new char[logLength];
        glGetProgramInfoLog(object(), 0xffff, NULL, logc);
        log=logc;
        delete logc;
      }
      return log;
    }
    bool validate()
    {
      glValidateProgram(object());
      GLint status;
      glGetProgramiv(object(), GL_VALIDATE_STATUS, &status);
      return (bool)status;
    }
    ~Program() { if (hasObject()) glDeleteProgram(object()); }
    Program(Program&&) = default;
    Program& operator=(Program&&) = default;
};

class Texture : GLObject {
  public:
    using GLObject::GLObject;
    Texture(glm::ivec3 size, const uint8_t* texels) : GLObject()
    {
      glCreateTextures(GL_TEXTURE_2D_ARRAY,1,objectPtr()); 
      glBindTextureUnit(0, object());
      glTextureParameteri(object(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTextureParameteri(object(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTextureParameteri(object(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTextureParameteri(object(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTextureStorage3D(object(), 1, GL_RGBA8, size[0], size[1], size[2]);
      glTextureSubImage3D(object(), 0, 0, 0, 0, size[0], size[1], size[2],
          GL_RGBA, GL_UNSIGNED_BYTE, texels);

    }
    ~Texture() { if (hasObject()) glDeleteTextures(1, objectPtr());}
    Texture(Texture&&) = default;
    Texture& operator=(Texture&&) = default;

    void bind()
    {
      glBindTextureUnit(0, object());
    }

    int width()
    {
      GLint w;
      glGetTextureLevelParameteriv(object(),0,GL_TEXTURE_WIDTH,&w);
      return w;
    }
    int height()
    {
      GLint h;
      glGetTextureLevelParameteriv(object(),0,GL_TEXTURE_HEIGHT,&h);
      return h;
    }
    int layerCount()
    {
      GLint l;
      glGetTextureLevelParameteriv(object(),0,GL_TEXTURE_DEPTH,&l);
      return l;
    }
};

template<typename... Ts>
class VertexArray; 

template <typename... As>
class VertexBuffer : GLObject {
  friend VertexArray<As...>;
  public:
  using GLObject::GLObject;
  using Types = attributes<As...>::types;
  using TupleType = typename meta::apply<SLTuple::Tuple, Types>::value;
  const size_t stride = meta::sizeofall<Types>;
  VertexBuffer(GLenum usage) : GLObject(), usage(usage)
  {
    glCreateBuffers(1, objectPtr());
  }
  void bufferData(size_t size, const TupleType* data)
  {
    glNamedBufferData(object(), stride*size, data, usage);
  }
  ~VertexBuffer() { if (hasObject()) glDeleteBuffers(1,objectPtr()); }
  VertexBuffer(VertexBuffer&&) = default;
  VertexBuffer& operator=(VertexBuffer&&) = default;
  private:
  GLenum usage;
};


template<typename... As>
class VertexArray : GLObject {
  using attributeType = attributes<As...>;
  public:
  using GLObject::GLObject;
  using bufferType = VertexBuffer<As...>;
  VertexArray() : GLObject()
  {
    glGenVertexArrays(1, objectPtr());
    glBindVertexArray(object());
    attributeType a(object());
  }
  bufferType newBuffer(GLuint offset, GLenum usage)
  {
    bufferType buffer(usage);
    bindBuffer(buffer, offset);
    return buffer;
  }
  void bindBuffer(bufferType& buffer, GLuint offset)
  {
    glVertexArrayVertexBuffer(object(), 0, buffer.object(), offset, buffer.stride);
  }
  ~VertexArray() { if (hasObject()) glDeleteVertexArrays(1, objectPtr()); }
  VertexArray(VertexArray&&) = default;
  VertexArray& operator=(VertexArray&&) = default;
};

class SDLContext : public Raii {
  using Raii::Raii;
  public:
  SDLContext(SDL_GLContext c) : Raii(make_any<SDL_GLContext>(c)) {}
  SDL_GLContext object() { return any_cast<SDL_GLContext>(object_); }
  ~SDLContext() { if (hasObject()) SDL_GL_DeleteContext(object()); }
  SDLContext(SDLContext&&) = default;
  SDLContext& operator=(SDLContext&&) = default;
};

class Window : public Raii {
  using Raii::Raii;
  public:
  Window(const char *title, glm::ivec2 position, glm::ivec2 size, uint32_t flags)
    : Raii(make_any<SDL_Window*>(SDL_CreateWindow(title, position[0], position[1], size[0], size[1], flags))),
    context(SDL_GL_CreateContext(object())) {}
  void swap() { SDL_GL_SwapWindow(object()); }
  void size(glm::ivec2 s) { SDL_SetWindowSize(object(), s[0], s[1]); }
  glm::ivec2 size() const
  {
    int w,h;
    SDL_GetWindowSize(object(), &w,&h);
    return {w,h};
  }

  SDL_Window* object() const { return any_cast<SDL_Window*>(object_); }
  ~Window() { if (hasObject()) SDL_DestroyWindow(object()); }
  Window(Window&&) = default;
  Window& operator=(Window&&) = default;
  private:
  SDLContext context;
};

void debugCallback( GLenum source, GLenum type, 
    GLuint id, GLenum severity,
        GLsizei length, const GLchar *message, const void *userParam)
    {
      switch (severity) {
        case 0x9146 : cout << "GL_DEBUG_SEVERITY_HIGH"; break;
        case 0x9147 : cout << "GL_DEBUG_SEVERITY_MEDIUM"; break;
        case 0x9148 : cout << "GL_DEBUG_SEVERITY_LOW"; break;
        case 0x826B : cout << "GL_DEBUG_SEVERITY_NOTIFICATION"; break;
      }
      cout << ": ";
      cerr.write(message, length);
      cerr << endl;
    }

class Context {
  public:
    Context(string title, glm::ivec2 size)
    {
      SDL_Init(SDL_INIT_EVERYTHING);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
      SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG); 
      Window win(title.c_str(), {0, 0}, size, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
      window = move(win);
      glewExperimental = GL_TRUE;
      glewInit();
      glEnable(GL_DEBUG_OUTPUT);
      glDebugMessageCallback(&debugCallback, nullptr);
    }
    ~Context() { SDL_Quit(); }
    Window window;
  private: 

};
#endif
