#include<glm/glm.hpp>
template<typename T>
class Uniform {
  public:
    Uniform(GLuint location) : location(location) {}
    Uniform(GLuint location, T value)
      : location(location) { set(value); }
    void set(T value);
  private:
    GLuint location;
};

template<typename T>
class Uniformv {
  public:
    Uniformv(GLuint location) : location(location) {}
    Uniformv(GLuint location, std::initializer_list<T> values)
      : location(location) { set(values); }
    void set(std::initializer_list<T> values);
  private:
    GLuint location;
};

  template<>
void Uniform<float>::set(float value)
{
  glUniform1f(location, value);
}

  template<>
void Uniform<glm::vec2>::set(glm::vec2 value)
{
  glUniform2f(location, value[0], value[1]);
}

  template<>
void Uniform<glm::vec3>::set(glm::vec3 value)
{
  glUniform3f(location, value[0], value[1], value[2]);
}

  template<>
void Uniform<glm::vec4>::set(glm::vec4 value)
{
  glUniform4f(location, value[0], value[1], value[2], value[3]);
}


  template<>
void Uniform<int>::set(int value)
{
  glUniform1i(location, value);
}

  template<>
void Uniform<glm::ivec2>::set(glm::ivec2 value)
{
  glUniform2i(location, value[0], value[1]);
}

  template<>
void Uniform<glm::ivec3>::set(glm::ivec3 value)
{
  glUniform3i(location, value[0], value[1], value[2]);
}

  template<>
void Uniform<glm::ivec4>::set(glm::ivec4 value)
{
  glUniform4i(location, value[0], value[1], value[2], value[3]);
}


  template<>
void Uniform<unsigned int>::set(unsigned int value)
{
  glUniform1ui(location, value);
}

  template<>
void Uniform<glm::uvec2>::set(glm::uvec2 value)
{
  glUniform2ui(location, value[0], value[1]);
}

  template<>
void Uniform<glm::uvec3>::set(glm::uvec3 value)
{
  glUniform3ui(location, value[0], value[1], value[3]);
}

  template<>
void Uniform<glm::uvec4>::set(glm::uvec4 value)
{
  glUniform4ui(location, value[0], value[1], value[2], value[3]);
}


  template<>
void Uniformv<float>::set(std::initializer_list<float> values)
{
  glUniform1fv(location, values.size(), (GLfloat*)std::data(values));
}

  template<>
void Uniformv<glm::vec2>::set(std::initializer_list<glm::vec2> values)
{
  glUniform2fv(location, values.size(), (GLfloat*)std::data(values));
}

  template<>
void Uniformv<glm::vec3>::set(std::initializer_list<glm::vec3> values)
{
  glUniform3fv(location, values.size(), (GLfloat*)std::data(values));
}

  template<>
void Uniformv<glm::vec4>::set(std::initializer_list<glm::vec4> values)
{
  glUniform4fv(location, values.size(), (GLfloat*)std::data(values));
}


  template<>
void Uniformv<int>::set(std::initializer_list<int> values)
{
  glUniform1iv(location, values.size(), (GLint*)std::data(values));
}

  template<>
void Uniformv<glm::ivec2>::set(std::initializer_list<glm::ivec2> values)
{
  glUniform2iv(location, values.size(), (GLint*)std::data(values));
}

  template<>
void Uniformv<glm::ivec3>::set(std::initializer_list<glm::ivec3> values)
{
  glUniform3iv(location, values.size(), (GLint*)std::data(values));
}

  template<>
void Uniformv<glm::ivec4>::set(std::initializer_list<glm::ivec4> values)
{
  glUniform4iv(location, values.size(), (GLint*)std::data(values));
}


  template<>
void Uniformv<unsigned int>::set(std::initializer_list<unsigned int> values)
{
  glUniform1uiv(location, values.size(), (GLuint*)std::data(values));
}

  template<>
void Uniformv<glm::uvec2>::set(std::initializer_list<glm::uvec2> values)
{
  glUniform2uiv(location, values.size(), (GLuint*)std::data(values));
}

  template<>
void Uniformv<glm::uvec3>::set(std::initializer_list<glm::uvec3> values)
{
  glUniform3uiv(location, values.size(), (GLuint*)std::data(values));
}

  template<>
void Uniformv<glm::uvec4>::set(std::initializer_list<glm::uvec4> values)
{
  glUniform4uiv(location, values.size(), (GLuint*)std::data(values));
}


  template<>
void Uniformv<glm::mat2>::set(std::initializer_list<glm::mat2> values)
{
  glUniformMatrix2fv(location, values.size(), false, (GLfloat*)std::data(values));
}

  template<>
void Uniform<glm::mat2>::set(glm::mat2 value)
{
  glUniformMatrix2fv(location, 1, false, (GLfloat*)&value);
}

  template<>
void Uniformv<glm::mat3>::set(std::initializer_list<glm::mat3> values)
{
  glUniformMatrix3fv(location, values.size(), false, (GLfloat*)std::data(values));
}

  template<>
void Uniform<glm::mat3>::set(glm::mat3 value)
{
  glUniformMatrix3fv(location, 1, false, (GLfloat*)&value);
}

  template<>
void Uniformv<glm::mat4>::set(std::initializer_list<glm::mat4> values)
{
  glUniformMatrix4fv(location, values.size(), false, (GLfloat*)std::data(values));
}

  template<>
void Uniform<glm::mat4>::set(glm::mat4 value)
{
  glUniformMatrix4fv(location, 1, false, (GLfloat*)&value);
}

  template<>
void Uniformv<glm::mat2x3>::set(std::initializer_list<glm::mat2x3> values)
{
  glUniformMatrix2x3fv(location, values.size(), false, (GLfloat*)std::data(values));
}

  template<>
void Uniform<glm::mat2x3>::set(glm::mat2x3 value)
{
  glUniformMatrix2x3fv(location, 1, false, (GLfloat*)&value);
}

  template<>
void Uniformv<glm::mat3x2>::set(std::initializer_list<glm::mat3x2> values)
{
  glUniformMatrix3x2fv(location, values.size(), false, (GLfloat*)std::data(values));
}

  template<>
void Uniform<glm::mat3x2>::set(glm::mat3x2 value)
{
  glUniformMatrix3x2fv(location, 1, false, (GLfloat*)&value);
}

  template<>
void Uniformv<glm::mat2x4>::set(std::initializer_list<glm::mat2x4> values)
{
  glUniformMatrix2x4fv(location, values.size(), false, (GLfloat*)std::data(values));
}

  template<>
void Uniform<glm::mat2x4>::set(glm::mat2x4 value)
{
  glUniformMatrix2x4fv(location, 1, false, (GLfloat*)&value);
}

  template<>
void Uniformv<glm::mat4x2>::set(std::initializer_list<glm::mat4x2> values)
{
  glUniformMatrix4x2fv(location, values.size(), false, (GLfloat*)std::data(values));
}

  template<>
void Uniform<glm::mat4x2>::set(glm::mat4x2 value)
{
  glUniformMatrix4x2fv(location, 1, false, (GLfloat*)&value);
}

  template<>
void Uniformv<glm::mat3x4>::set(std::initializer_list<glm::mat3x4> values)
{
  glUniformMatrix3x4fv(location, values.size(), false, (GLfloat*)std::data(values));
}

  template<>
void Uniform<glm::mat3x4>::set(glm::mat3x4 value)
{
  glUniformMatrix3x4fv(location, 1, false, (GLfloat*)&value);
}

  template<>
void Uniformv<glm::mat4x3>::set(std::initializer_list<glm::mat4x3> values)
{
  glUniformMatrix4x3fv(location, values.size(), false, (GLfloat*)std::data(values));
}

  template<>
void Uniform<glm::mat4x3>::set(glm::mat4x3 value)
{
  glUniformMatrix4x3fv(location, 1, false, (GLfloat*)&value);
}
