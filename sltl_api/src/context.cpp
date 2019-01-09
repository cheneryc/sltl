#include "context.h"

#define GLEW_STATIC

#include <gl/glew.h>
#include <gl/wglew.h>

#include <exception>
#include <iostream>
#include <vector>

#include <cassert>


namespace
{
  namespace ns = sltl::api;

  void create_context(HWND wnd)
  {
    HDC dc = ::GetDC(wnd);

    //TODO: what about using PFD_DRAW_TO_BITMAP?
    PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
      PFD_TYPE_RGBA,
      32,                   // colour depth of the frame buffer
      0, 0, 0, 0, 0, 0,     // not used
      0,                    // alpha bits
      0,                    // not used
      0,                    // accum bits
      0, 0, 0, 0,           // not used
      24,                   // number of bits for the depth buffer
      8,                    // number of bits for the stencil buffer
      0,                    // aux buffers
      PFD_MAIN_PLANE,
      0, 0, 0, 0            // not used
    };

    int pf = ::ChoosePixelFormat(dc, &pfd);

    if(!pf)
    {
      throw std::exception();//TODO: better error handling...
    }

    //TODO: check that the returned pixel format is acceptable and matches that requested

    BOOL result = ::SetPixelFormat(dc, pf, &pfd);

    if(!result)
    {
      throw std::exception();//TODO: better error handling...
    }

    HGLRC glrc = ::wglCreateContext(dc);

    if(!::wglMakeCurrent(dc, glrc))
    {
      throw std::exception();//TODO: better error handling...
    }

    if(glewInit() != GLEW_OK)
    {
      throw std::exception();//TODO: better error handling...
    }

    wglMakeCurrent(dc, 0);
    wglDeleteContext(glrc);

    int attribs[] = {
      WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
      WGL_CONTEXT_MINOR_VERSION_ARB, 3,
      WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
      0,
    };

    glrc = ::wglCreateContextAttribsARB(dc, NULL, attribs);

    if(!glrc)
    {
      throw std::exception();//TODO: better error handling...
    }

    if(!::wglMakeCurrent(dc, glrc))
    {
      throw std::exception();//TODO: better error handling...
    }
  }

  LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
  {
    LRESULT result = 0;

    switch(msg)
    {
      case WM_CREATE:
        create_context(wnd);
        break;
      default:
        result = ::DefWindowProc(wnd, msg, wparam, lparam);
    }

    return result;
  }

  ATOM register_class()
  {
    WNDCLASSEX wc = {};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = &WndProc;
    wc.hInstance = ::GetModuleHandle(NULL);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND);
    wc.lpszClassName = L"SLTL_GL_WND_CLASS";

    return RegisterClassEx(&wc);
  }
}

struct ns::shader::impl
{
  GLuint _name;
};

ns::shader::shader(const char* shader_src, shader_stage stage) : _impl(new impl)
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
    throw std::exception();//TODO: better error handling...
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
    throw std::exception();//TODO: better error handling...
  }

  //1. test that the shaders are the stages they say they are...

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

struct ns::context::impl
{
  HWND _wnd;
};

ns::context::context() : _impl(new impl)
{
  static ATOM s_wc = ::register_class();

  if(s_wc)
  {
    _impl->_wnd = CreateWindowEx(0, MAKEINTATOM(s_wc), L"SLTL_GL_WND", 0, 0, 0, 0, 0, NULL, NULL, ::GetModuleHandle(NULL), NULL);

    if(_impl->_wnd == NULL)
    {
      throw std::exception();//TODO: better error handling...
    }
  }
}

ns::context::~context()
{
  ::DestroyWindow(_impl->_wnd);

  delete _impl;
}
