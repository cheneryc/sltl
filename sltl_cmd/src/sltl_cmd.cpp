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
    using namespace sltl;

    sltl::vector<float, 2> vec;
    sltl::vector<float, 3> vec1;
    sltl::vector<float, 3> vec2(sltl::vector<float, 2>(1.0f, 1.0f), 1.0f);
    sltl::vector<float, 3> vec3(vec1);
    sltl::vector<float, 3> vec4(vec, 1.0f);
    sltl::vector<float, 4> vec5(1.0f, 1.0f, 1.0f, 1.0f);

    //vec2 += vec1;

    //sclr_var++;//Is this normally defined for non-integral types (appears to work for all arithmetic types plus pointers)?

    return 0;
  }
}

int main(/*int argc, wchar_t* argv[]*/)
{
  auto shader = sltl::make_shader(sltl::shader::vertex, &build_shader);

  std::wstring shader_txt = shader.str<sltl::output>();
  std::wcout << shader_txt.c_str() << std::endl;

  return 0;
}
