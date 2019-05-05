#include "shader_gl.h"

#define GLEW_STATIC

#include <gl/glew.h>

#include <vector>
#include <exception>

#include <cassert>


namespace
{
  namespace ns = sltl::api::gl;
}

struct ns::shader::impl
{
  impl(sltl::api::shader_stage stage) : _stage(stage) {}

  const sltl::api::shader_stage _stage;
  GLuint _name;
};

ns::shader::shader(const char* shader_src, sltl::api::shader_stage stage) : _impl(new impl(stage))
{
  GLenum shader_type;

  switch(stage)
  {
    case shader_stage::vertex:
      shader_type = GL_VERTEX_SHADER;
      break;
    case shader_stage::fragment:
      shader_type = GL_FRAGMENT_SHADER;
      break;
    default:
      assert(stage == shader_stage::vertex || stage == shader_stage::fragment);
  }

  _impl->_name = glCreateShader(shader_type);

  if (!_impl->_name)
  {
    throw std::exception();//TODO: exception type and message
  }

  glShaderSource(_impl->_name, 1, &shader_src, nullptr);
  glCompileShader(_impl->_name);

  GLint compile_status = GL_FALSE;
  glGetShaderiv(_impl->_name, GL_COMPILE_STATUS, &compile_status);

  if(compile_status == GL_FALSE)
  {
    GLint log_length_buffer = 0;
    GLint log_length_actual = 0;

    glGetShaderiv(_impl->_name, GL_INFO_LOG_LENGTH, &log_length_buffer);
    std::vector<char> compile_log(log_length_buffer, '\0');
    glGetShaderInfoLog(_impl->_name, log_length_buffer, &log_length_actual, compile_log.data());

    assert(log_length_buffer == log_length_actual + 1); // 'buffer' length includes null terminator, 'actual' length does not

    glDeleteShader(_impl->_name);

    throw std::runtime_error(compile_log.data());
  }
}

ns::shader::~shader()
{
  glDeleteShader(_impl->_name);

  delete _impl;
}

struct ns::shader_program::impl
{
  GLuint _name;
};

ns::shader_program::shader_program(const shader& shader_vertex, const shader& shader_fragment) : _impl(new impl)
{
  _impl->_name = glCreateProgram();

  if(!_impl->_name)
  {
    throw std::exception();//TODO: exception type and message
  }

  if(shader_vertex._impl->_stage != api::shader_stage::vertex)
  {
    throw std::exception();//TODO: exception type and message
  }

  if(shader_fragment._impl->_stage != api::shader_stage::fragment)
  {
    throw std::exception();//TODO: exception type and message
  }

  glAttachShader(_impl->_name, shader_vertex._impl->_name);
  glAttachShader(_impl->_name, shader_fragment._impl->_name);

  glLinkProgram(_impl->_name);

  GLint link_status = GL_FALSE;
  glGetProgramiv(_impl->_name, GL_LINK_STATUS, &link_status);

  if(link_status == GL_FALSE)
  {
    GLint log_length_buffer = 0;
    GLint log_length_actual = 0;

    glGetProgramiv(_impl->_name, GL_INFO_LOG_LENGTH, &log_length_buffer);
    std::vector<char> link_log(log_length_buffer, '\0');
    glGetProgramInfoLog(_impl->_name, log_length_buffer, &log_length_actual, link_log.data());

    assert(log_length_buffer == log_length_actual + 1); // 'buffer' length includes null terminator, 'actual' length does not

    glDeleteProgram(_impl->_name);

    throw std::runtime_error(link_log.data());
  }

  glDetachShader(_impl->_name, shader_vertex._impl->_name);
  glDetachShader(_impl->_name, shader_fragment._impl->_name);
}

ns::shader_program::~shader_program()
{
  glDeleteProgram(_impl->_name);

  delete _impl;
}
