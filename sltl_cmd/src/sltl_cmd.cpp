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

    scalar<float> sclr_var, sclr_var2;
    sclr_var += sclr_var + sclr_var2;

    /*{
      scope block_scope(scope::block);

      scalar<float> sclr_float;
      scalar<double> sclr_double;
      scalar<int> sclr_var_int;
      scalar<unsigned int> sclr_var_uint;
      scalar<bool> sclr_var_bool;
    }

    sclr_var = sclr_var2;// This most basic case is working
    sclr_var = scalar<float>();// This works if only the default constructor is called
    sclr_var = scalar<float>(2.2f);
    sclr_var = 3.21f;//TODO: fix, currently wraps the 3.21f in a float constructor (i.e. = float(3.21f))

    //TODO: this case will not work correctly unless the assignment operator returns a scalar_ref value_type rather than a scalar&
    // The scalar_ref would act as a proxy to the assignment operation node and possibly allow this construct to be recreated correctly in the shader
    sclr_var = sclr_var2 = sclr_var;

    // Scalar definition
    scalar<bool> def0 = scalar<bool>(true);
    scalar<bool> def1(false);
    scalar<bool> def1b = false;
    scalar<float> def2 = sclr_var;
    scalar<float> def3(sclr_var);
    scalar<float> def4(scalar<float>(1.0f));//This is behaving strangely! Seems to just remove the temp variable

    sclr_var += (sclr_var + 1.0f) + scalar<float>(1.0f);//TODO: need brackets adding for compound expressions
    //sclr_var++;//Is this normally defined for non-integral types (appears to work for all arithmetic types plus pointers)?

    //TODO: the if, else if and else lines have semi-colons when they shouldn't!
    make_if(def0, [&sclr_var]()
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
