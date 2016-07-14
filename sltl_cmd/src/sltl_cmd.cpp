#include "shader.h"
#include "output.h"

#include "io/io.h"

#include "core/qualifier.h"
#include "core/shader_stage.h"

#include <iostream>


namespace
{
  using namespace sltl;

  io::block<> shader_fn(shader::tag<core::shader_stage::test>, io::block<>)
  {
    return io::block<>(core::qualifier_storage::out);
  }
}

int main()
{
  auto shader = sltl::make_shader(&shader_fn);

  std::wstring shader_txt = shader.str<sltl::output>();
  std::wcout << shader_txt.c_str() << std::endl;

  return 0;
}
