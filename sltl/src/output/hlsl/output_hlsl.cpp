#include "output_hlsl.h"

#include <syntax/block.h>
#include <syntax/io_block.h>
#include <syntax/reference.h>
#include <syntax/function_definition.h>
#include <syntax/variable_declaration.h>
#include <syntax/parameter_declaration.h>

#include <output/language.h>

#include <type.h>


namespace
{
  namespace ns = sltl::hlsl;

  bool is_system_value_semantic(sltl::core::semantic semantic, sltl::core::semantic_index_t semantic_index)
  {
    bool is_system_value = false;

    if(semantic == sltl::core::semantic::system)
    {
      const auto semantic_system_pair = sltl::core::detail::to_semantic_system_pair(semantic_index);

      switch(semantic_system_pair.first)
      {
        // Only a built-in variable if the semantic is position or depth
        // Only a built-in variable if the index is zero
        case sltl::core::semantic_system::position:
        case sltl::core::semantic_system::depth:
          is_system_value = !semantic_system_pair.second;
          break;
      }
    }

    return is_system_value;
  }

  bool is_system_value_semantic(const sltl::syntax::variable_declaration& vd)
  {
    return is_system_value_semantic(vd._semantic, vd._semantic_index);
  }

  std::wstring to_system_value_semantic_string(sltl::core::semantic semantic, sltl::core::semantic_index_t semantic_index)
  {
    assert(is_system_value_semantic(semantic, semantic_index));

    const auto semantic_system_pair = sltl::core::detail::to_semantic_system_pair(semantic_index);

    //TODO: validation that the built-in is of the correct type and used in the correct shader stage

    std::wstringstream ss;

    ss << L"SV_";

    switch(semantic_system_pair.first)
    {
    case sltl::core::semantic_system::position:
      //TODO: note that index must be zero for position semantic
      ss << L"Position";
      break;
    case sltl::core::semantic_system::depth:
      //TODO: only valid as a fragment shader output
      //TODO: note that index must be zero for depth semantic
      ss << L"Depth";
      break;
    }

    return ss.str();
  }

  std::wstring to_system_value_semantic_string(const sltl::syntax::variable_declaration& vd)
  {
    return to_system_value_semantic_string(vd._semantic, vd._semantic_index);
  }

  std::wstring to_type_string(const sltl::language::type& t)
  {
    using namespace sltl;

    std::wstringstream ss;

    const language::type_id id = t.get_id();
    const language::type_dimensions& dimensions = t.get_dimensions();

    if(dimensions.is_void())
    {
      assert(id == language::id_void);
      assert(dimensions.m() == 0U);
      assert(dimensions.n() == 0U);

      ss << L"void";
    }
    else
    {
      switch(id)
      {
        case language::id_float:
          ss << L"float";
          break;
        case language::id_double:
          ss << L"double";
          break;
        case language::id_int:
          ss << L"int";
          break;
        case language::id_uint:
          ss << L"uint";
          break;
        case language::id_bool:
          ss << L"bool";
          break;
        default:
          assert((id != language::id_unknown) && (id != language::id_void));
      }

      if(dimensions.is_vector())
      {
        ss << (is_row_vector(dimensions) ? dimensions.n() : dimensions.m());
      }

      if(dimensions.is_matrix())
      {
        // HLSL has row-major matrices so always output 'mxn'
        ss << dimensions.m() << L'x' << dimensions.n();
      }
    }

    return ss.str();
  }

  std::wstring to_type_string(const sltl::syntax::io_block& iob, sltl::core::shader_stage stage)
  {
    std::wstringstream ss;

    switch(stage)
    {
      case sltl::core::shader_stage::vertex:
        ss << L"vs";
        break;
      case sltl::core::shader_stage::geometry:
        ss << L"gs";
        break;
      case sltl::core::shader_stage::fragment:
        ss << L"ps";
        break;
      default:
        assert(stage == sltl::core::shader_stage::test);
    }

    if(stage != sltl::core::shader_stage::test)
    {
      ss << L'_';
    }

    switch(iob._qualifier)
    {
      case sltl::core::qualifier_storage::in:
        ss << L"input";
        break;
      case sltl::core::qualifier_storage::out:
        ss << L"output";
        break;
      case sltl::core::qualifier_storage::uniform:
        //TODO: constant buffer...
        break;
    }

    return ss.str();
  }

  std::wstring to_type_prefix_string(const sltl::language::type& t)
  {
    using namespace sltl;

    // Variable names can't begin with a numeral so we need a type specific prefix
    std::wstring prefix_string;

    const language::type_id id = t.get_id();
    const language::type_dimensions& dimensions = t.get_dimensions();

    assert(id != language::id_void);
    assert(id != language::id_unknown);

    if(dimensions.is_scalar())
    {
      switch(id)
      {
        case language::id_float:
          prefix_string = L'f';
          break;
        case language::id_double:
          prefix_string = L'd';
          break;
        case language::id_int:
          prefix_string = L'i';
          break;
        case language::id_uint:
          prefix_string = L'u';
          break;
        case language::id_bool:
          prefix_string = L'b';
          break;
        default:
          assert((id != language::id_unknown) && (id != language::id_void));
      }
    }
    else if(dimensions.is_vector())
    {
      assert(id != language::id_void);
      assert(id != language::id_unknown);

      prefix_string = L'v';
    }
    else if(dimensions.is_matrix())
    {
      assert(id != language::id_void);
      assert(id != language::id_unknown);

      prefix_string = L'm';
    }
    else
    {
      assert(false);
    }

    return prefix_string;
  }

  const wchar_t* to_parameter_prefix_string(sltl::core::qualifier_param id)
  {
    switch(id)
    {
      case sltl::core::qualifier_param::in:
      case sltl::core::qualifier_param::inout:
      case sltl::core::qualifier_param::out:
        return L"p";
    }

    return nullptr;
  }

  const wchar_t* to_qualifier_prefix_string(sltl::core::qualifier_storage id)
  {
    switch(id)
    {
    case sltl::core::qualifier_storage::in:
      return L"in";
    case sltl::core::qualifier_storage::out:
      return L"out";
    }

    return nullptr;
  }

  std::wstring get_variable_name(const sltl::syntax::variable_declaration& vd)
  {
    std::wstringstream ss;

    ss << ::to_type_prefix_string(vd.get_type());
    ss << vd._name;

    return ss.str();
  }
}

ns::output_hlsl::output_hlsl(sltl::core::shader_stage stage, sltl::detail::enum_flags<sltl::output_flags> flags) : output(stage, flags),
  _block_in(nullptr),
  _block_out(nullptr)
{
}

sltl::syntax::action_return_t ns::output_hlsl::operator()(const sltl::syntax::io_block& iob, bool is_start)
{
  if(!iob.is_empty())
  {
    if(is_start)
    {
      // Store the pointers to the 'in' and 'out' io_blocks for use later while processing the 'main' function definition
      switch(iob._qualifier)
      {
        case core::qualifier_storage::in:
          _block_in = &iob;
          break;
        case core::qualifier_storage::out:
          _block_out = &iob;
          break;
      }

      line_begin(indent_t::current);

      _ss << to_keyword_string(language::id_struct) << L' ';
      _ss << to_type_string(iob, _stage);

      line_end(false);
    }

    line_begin(is_start ? indent_t::increase : indent_t::decrease);
    output::operator()(language::bracket_tag<language::id_brace>(), is_start);
    line_end(!is_start);
  }

  return is_start ? syntax::action_return_t::step_in :
                    syntax::action_return_t::step_out;
}

sltl::syntax::action_return_t ns::output_hlsl::operator()(const sltl::syntax::reference& r)
{
  if(auto vd = dynamic_cast<const syntax::variable_declaration*>(&r._declaration))
  {
    // Prefix input, output and uniform variables with a struct or cbuffer name (followed by a period)
    if(auto prefix = ::to_qualifier_prefix_string(vd->_qualifier))
    {
      _ss << prefix << L'.';
    }
  }

  return output::operator()(r);
}

sltl::syntax::action_return_t ns::output_hlsl::operator()(const sltl::syntax::variable_declaration& vd, bool is_start)
{
  if(is_start)
  {
    line_begin(indent_t::current);

    _ss << get_type_name(vd.get_type()) << L' ';
    _ss << get_variable_name(vd);

    if(::is_system_value_semantic(vd))
    {
      _ss << L" : " << ::to_system_value_semantic_string(vd);
    }
    else if(vd.has_initializer())
    {
      _ss << L" = ";
    }
  }
  else
  {
    line_end();
  }

  return is_start ? syntax::action_return_t::step_in :
                    syntax::action_return_t::step_out;
}

sltl::syntax::action_return_t ns::output_hlsl::operator()(const sltl::syntax::function_definition& fd, bool is_start)
{
  syntax::action_return_t return_val;

  if((fd._name == L"main") && is_start)
  {
    assert(fd.get_type() == language::type_helper<void>());
    assert(fd.get_params().size() == 0U);

    line_begin(indent_t::current);

    // Replace the main function's 'void' return type with the output io_block's type
    if(_block_out)
    {
      _ss << ::to_type_string(*_block_out, _stage);
    }
    else
    {
      _ss << get_type_name(language::type_helper<void>());
    }

    _ss << L' ' << fd._name << L'(';

    // Replace the main function's empty parameter list with an input io_block parameter
    if(_block_in)
    {
      _ss << ::to_type_string(*_block_in, _stage) << L' ' << ::to_qualifier_prefix_string(_block_in->_qualifier);
    }

    //TODO: need to instantiate an instance of the output io_block in the main function
    //TODO: need to return this instance of the output io_block as the, presumably final, statement of the main function

    //1. Get the function's block
    //2. Output an opening brace + newlines and indents
    //3. Output a variable declaration for the output io_block
    //4. Manually visit the block's children by calling apply_action directly (like the parameter_list)
    //5. Output a return statement for the main function
    //6. Output a closing brace + newlines and indents
    //7. Return 'step_over'

    return_val = syntax::action_return_t::step_in;
  }
  else
  {
    return_val = output::operator()(fd, is_start);
  }

  return return_val;
}

std::wstring ns::output_hlsl::get_type_name(const sltl::language::type& type) const
{
  return ::to_type_string(_flags.has_flag<output_flags::flag_transpose_type>() ? type.transpose() : type);
}

std::wstring ns::output_hlsl::get_variable_name(const sltl::syntax::variable_declaration& vd) const
{
  return ::get_variable_name(vd);
}

std::wstring ns::output_hlsl::get_parameter_name(const sltl::syntax::parameter_declaration& pd) const
{
  std::wstringstream ss(::to_parameter_prefix_string(pd._qualifier), std::ios::in | std::ios::out | std::ios::ate);

  ss << L'_';
  ss << ::to_type_prefix_string(pd.get_type());
  ss << pd._name;

  return ss.str();
}
