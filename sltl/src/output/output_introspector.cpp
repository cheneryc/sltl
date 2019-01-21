#include "output_introspector.h"

#include "glsl/glsl_convention.h"

#include <syntax/io_block.h>
#include <syntax/variable_declaration.h>


namespace
{
  namespace ns = sltl;
}

ns::output_introspector::output_introspector(core::shader_stage, core::qualifier_storage qualifier, core::semantic_pair semantic) :
  _semantic(semantic._semantic),
  _semantic_index(semantic._index),
  _qualifier(qualifier)
{
}

ns::syntax::action_return_t ns::output_introspector::operator()(const syntax::io_block& iob, bool is_start)
{
  ns::syntax::action_return_t return_val;

  if(is_start)
  {
    return_val = ((iob._qualifier == _qualifier) ?
      ns::syntax::action_return_t::step_in :
      ns::syntax::action_return_t::step_over);
  }
  else
  {
    return_val = ns::syntax::action_return_t::step_out;
  }

  return return_val;
}

ns::syntax::action_return_t ns::output_introspector::operator()(const syntax::variable_declaration& vd, bool is_start)
{
  ns::syntax::action_return_t return_val = (is_start ?
    ns::syntax::action_return_t::step_over :
    ns::syntax::action_return_t::step_out);

  if(is_start)
  {
    if((vd._qualifier == _qualifier) &&
       (vd._semantic == _semantic) &&
       (vd._semantic_index == _semantic_index))
    {
      _name = glsl::get_variable_name(vd); return_val = ns::syntax::action_return_t::stop;
    }
  }

  return return_val;
}

std::wstring ns::output_introspector::get_result() const
{
  return _name;
}

ns::syntax::action_return_t ns::output_introspector::get_default(bool is_start)
{
  return is_start ? ns::syntax::action_return_t::step_in :
                    ns::syntax::action_return_t::step_out;
}
