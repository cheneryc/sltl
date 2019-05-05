#include "context.h"

#define GLEW_STATIC

#include <gl/glew.h>
#include <gl/wglew.h>

#include <exception>


namespace
{
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

  namespace ns = sltl::api::gl;
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
