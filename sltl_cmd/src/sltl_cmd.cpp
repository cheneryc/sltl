#include "shader.h"

#include <iostream>


namespace
{
  using namespace sltl;

  void shader_fn(shader::tag<shader::test>)
  {
  }
}

int main()
{
  auto shader = sltl::make_shader(&shader_fn);

  std::wstring shader_txt = shader.str<sltl::output>();
  std::wcout << shader_txt.c_str() << std::endl;

  return 0;
}
