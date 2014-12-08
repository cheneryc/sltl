#include "shader.h"
#include "vector.h"
#include "scope.h"
#include "output.h"
#include "if.h"

#include <iostream>


namespace
{
  //TODO: need to figure out what the return value should be
  int build_shader()
  {
    return 0;
  }
}

int main()
{
  auto shader = sltl::make_shader(sltl::shader::vertex, &build_shader);

  std::wstring shader_txt = shader.str<sltl::output>();
  std::wcout << shader_txt.c_str() << std::endl;

  return 0;
}
