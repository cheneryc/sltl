#include "shader.h"

#include "io/io.h"

#include <iostream>


namespace
{
  using namespace sltl;

  io::block<> shader_fn(shader::tag<shader::test>, io::block<>)
  {
    return io::block<>(io::qualifier::out);
  }
}

int main()
{
  auto shader = sltl::make_shader(&shader_fn);

  std::wstring shader_txt = shader.str<sltl::output>();
  std::wcout << shader_txt.c_str() << std::endl;

  return 0;
}
