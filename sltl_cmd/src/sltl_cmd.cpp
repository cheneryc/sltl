#include "shader.h"
#include "scalar.h"
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

    //sclr_var++;//Is this normally defined for non-integral types (appears to work for all arithmetic types plus pointers)?

    //TODO: rename the functions if_, else_ and else_if?
    //TODO: the if, else if and else lines have semi-colons when they shouldn't!
    /*make_if(def0, [&sclr_var]()
    {
      scalar<float> var_if = 5.5555f;
      var_if += sclr_var;
    }).make_else_if(def1, []()
    {
    }).make_else([]()
    {
    });*/

    return 0;
  }
}

int main(int argc, wchar_t* argv[])
{
  auto shader = sltl::make_shader(sltl::shader::vertex, &build_shader);

  std::wstring shader_txt = shader.str<sltl::output>();
  std::wcout << shader_txt.c_str() << std::endl;

  return 0;
}
