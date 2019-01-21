#include "output_matrix_order.h"

#include <syntax/operator.h>
#include <syntax/variable_declaration.h>
#include <syntax/temporary.h>
#include <syntax/constructor_call.h>
#include <syntax/function_definition.h>


namespace
{
  namespace ns = sltl;
}

ns::output_matrix_order::output_matrix_order(core::shader_stage)
{
}

ns::syntax::action_return_t ns::output_matrix_order::operator()(syntax::operator_binary& ob, bool is_start)
{
  ns::syntax::action_return_t return_val = ns::syntax::action_return_t::step_out;

  if(is_start)
  {
    return_val = ns::syntax::action_return_t::step_in;

    if(ob._operator_id == language::id_matrix_multiplication)
    {
      const language::type type_lhs = ob._operand_lhs->get_type();
      const language::type type_rhs = ob._operand_rhs->get_type();

      // matrix-matrix and vector-matrix multiplication need converting from row-order to column-order
      if((type_lhs.get_dimensions().is_matrix() || type_lhs.get_dimensions().is_vector()) &&
         (type_rhs.get_dimensions().is_matrix() || type_rhs.get_dimensions().is_vector()))
      {
        // both operands cannot be vectors
        assert(!(type_lhs.get_dimensions().is_vector() &&
                 type_rhs.get_dimensions().is_vector()));

        ob.swap_operands();
      }
    }
  }

  return return_val;
}

ns::syntax::action_return_t ns::output_matrix_order::operator()(syntax::operator_component_access& oca, bool is_start)
{
  //TODO: implement this...
  assert(false);
  return ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output_matrix_order::operator()(syntax::variable_declaration& vd, bool is_start)
{
  ns::syntax::action_return_t return_val = ns::syntax::action_return_t::step_out;

  if(is_start)
  {
    const bool is_transposable = vd.has_type();

    if(is_transposable)
    {
      vd.set_type(vd.get_type().transpose());
    }

    return_val = is_transposable ?
      ns::syntax::action_return_t::step_over :
      ns::syntax::action_return_t::step_in;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output_matrix_order::operator()(syntax::temporary& t, bool is_start)
{
  ns::syntax::action_return_t return_val = ns::syntax::action_return_t::step_out;

  if(is_start)
  {
    const bool is_transposable = t.has_type();

    if(is_transposable)
    {
      t.set_type(t.get_type().transpose());
    }

    return_val = is_transposable ?
      ns::syntax::action_return_t::step_over :
      ns::syntax::action_return_t::step_in;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output_matrix_order::operator()(syntax::constructor_call& cc, bool is_start)
{
  if(is_start)
  {
    cc.set_type(cc.get_type().transpose());
  }

  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output_matrix_order::operator()(syntax::function_definition& fd, bool is_start)
{
  if(is_start)
  {
    fd.set_type(fd.get_type().transpose());
  }

  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}

ns::syntax::action_return_t ns::output_matrix_order::get_default(bool is_start)
{
  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}
