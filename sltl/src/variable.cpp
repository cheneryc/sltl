#include "variable.h"

#include "syntax/block.h"
#include "syntax/block_manager.h"
#include "syntax/reference.h"
#include "syntax/temporary.h"
#include "syntax/variable_info.h"
#include "syntax/variable_declaration.h"
#include "syntax/parameter_declaration.h"

#include <cassert>


namespace
{
  namespace ns = sltl;
}

ns::variable::variable(syntax::variable_declaration* vd) : _declaration(vd) {}
ns::variable::variable(syntax::parameter_declaration* pd) : _declaration(pd) {}

ns::syntax::expression::ptr ns::variable::make_reference() const
{
  if(auto vd = dynamic_cast<syntax::variable_declaration*>(_declaration))
  {
    get_variable_info(vd).inc_ref();
  }

  return syntax::expression::make<syntax::reference>(*_declaration);
}

ns::syntax::expression::ptr ns::variable::make_reference_or_temporary()
{
  if(auto vd = dynamic_cast<syntax::variable_declaration*>(_declaration))
  {
    if(get_variable_info(vd).get_ref() > 0)
    {
      return make_reference();
    }
    else
    {
      return make_temporary();
    }
  }
  else if(auto pd = dynamic_cast<syntax::parameter_declaration*>(_declaration))
  {
    return make_reference();
  }
  else
  {
    throw std::exception();//TODO: exception type and message
  }
}

ns::syntax::expression::ptr ns::variable::make_temporary()
{
  if(auto vd = dynamic_cast<syntax::variable_declaration*>(_declaration))
  {
    assert(get_variable_info(vd).get_ref() == 0);

    syntax::expression::ptr exp;

    {
      syntax::variable_declaration& declaration = *vd; _declaration = nullptr;

      exp = (declaration.has_type() ?
        syntax::expression::make<syntax::temporary>(declaration.get_type()) :
        syntax::expression::make<syntax::temporary>(declaration.move()));

      syntax::get_current_block().erase(declaration);
    }

    return std::move(exp);
  }
  else
  {
    throw std::exception();//TODO: exception type and message
  }
}

ns::syntax::variable_declaration* ns::create_variable_declaration(syntax::expression::ptr&& initializer)
{
  return &(syntax::get_current_block().add<syntax::variable_declaration>(std::move(initializer)));
}

ns::syntax::variable_declaration* ns::create_variable_declaration(const language::type& type, core::semantic_pair semantic)
{
  return &(syntax::get_current_block().add<syntax::variable_declaration>(type, semantic));
}
