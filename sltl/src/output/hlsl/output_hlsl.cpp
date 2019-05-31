#include "output_hlsl.h"

#include <syntax/block.h>
#include <syntax/io_block.h>
#include <syntax/reference.h>
#include <syntax/operator.h>
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
        case sltl::core::semantic_system::position:
        case sltl::core::semantic_system::depth:
          is_system_value = !semantic_system_pair.second; // Only a system value if the index is zero
          break;
        case sltl::core::semantic_system::target:
          is_system_value = true;
          break;
      }
    }

    return is_system_value;
  }

  bool is_system_value_semantic(const sltl::syntax::variable_declaration& vd)
  {
    return is_system_value_semantic(vd._semantic, vd._semantic_index);
  }

  std::wstring to_system_value_semantic_string(sltl::core::semantic_system semantic_system, sltl::core::semantic_index_t semantic_index)
  {
    //TODO: validation that the built-in is of the correct type and used in the correct shader stage

    std::wstringstream ss;

    ss << L"SV_";

    switch(semantic_system)
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
    case sltl::core::semantic_system::target:
      //TODO: only valid as a fragment shader output
      ss << L"Target";
      break;
    }

    return ss.str();
  }

  std::wstring to_semantic_string(sltl::core::semantic semantic, sltl::core::semantic_index_t semantic_index)
  {
    std::wstring semantic_string;

    if(is_system_value_semantic(semantic, semantic_index))
    {
      const auto semantic_system_pair = sltl::core::detail::to_semantic_system_pair(semantic_index);

      semantic_string = to_system_value_semantic_string(semantic_system_pair.first,
                                                        semantic_system_pair.second);
    }
    else
    {
      std::wstringstream ss;

      switch(semantic)
      {
      case sltl::core::semantic::position:
        ss << L"POSITION";
        break;
      case sltl::core::semantic::depth:
        ss << L"DEPTH";
        break;
      case sltl::core::semantic::normal:
        ss << L"NORMAL";
        break;
      case sltl::core::semantic::texcoord:
        ss << L"TEXCOORD";
        break;
      case sltl::core::semantic::colour:
        ss << L"COLOR";
        break;
      case sltl::core::semantic::material:
        ss << L"MATERIAL";
        break;
      case sltl::core::semantic::transform:
        ss << L"TRANSFORM";
        break;
      case sltl::core::semantic::camera:
        ss << L"CAMERA";
        break;
      case sltl::core::semantic::light:
        ss << L"LIGHT";
        break;
      case sltl::core::semantic::user:
        ss << L"USER";
        break;
      default:
        assert(false);
      }

      if(semantic_index)
      {
        ss << semantic_index;
      }

      semantic_string = ss.str();
    }

    return semantic_string;
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

    if(iob._qualifier == sltl::core::qualifier_storage::uniform)
    {
      ss << L"cb";
    }
    else
    {
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
        default:
          assert((iob._qualifier != sltl::core::qualifier_storage::none) && (iob._qualifier != sltl::core::qualifier_storage::uniform));
      }
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
      return L"i";
    case sltl::core::qualifier_storage::out:
      return L"o";
    case sltl::core::qualifier_storage::uniform:
      return L"c";
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
      if(_flags.has_flag<output_flags::flag_extra_newlines>())
      {
        _ss << get_newline();
      }

      const wchar_t* keyword = nullptr;

      // Store the pointers to the 'in' and 'out' io_blocks for use later while processing the 'main' function definition
      switch(iob._qualifier)
      {
        case core::qualifier_storage::in:
          _block_in = &iob;
          keyword = to_keyword_string(language::id_struct);
          break;
        case core::qualifier_storage::out:
          _block_out = &iob;
          keyword = to_keyword_string(language::id_struct);
          break;
        case core::qualifier_storage::uniform:
          keyword = L"cbuffer";
          break;
        default:
          assert(iob._qualifier != core::qualifier_storage::none);
      }

      // Output the struct keyword followed by the type name
      _ss << get_indent(indent_t::current);
      _ss << keyword << L' ' << to_type_string(iob, _stage);
      _ss << get_newline();

      // Output the opening brace
      _ss << get_indent(indent_t::increase);
      output::operator()(language::bracket_tag<language::id_brace>(), true);
      _ss << get_newline();
    }
    else
    {
      // Output the closing brace
      _ss << get_indent(indent_t::decrease);
      output::operator()(language::bracket_tag<language::id_brace>(), false);
      _ss << get_terminal_newline();
    }
  }

  return is_start ? syntax::action_return_t::step_in :
                    syntax::action_return_t::step_out;
}

sltl::syntax::action_return_t ns::output_hlsl::operator()(const sltl::syntax::reference& r)
{
  if(auto vd = dynamic_cast<const syntax::variable_declaration*>(&r._declaration))
  {
    // Prefix input, output and uniform variables with a struct or cbuffer name (followed by a period or underscore)
    if(auto prefix = ::to_qualifier_prefix_string(vd->_qualifier))
    {
      _ss << prefix << ((vd->_qualifier == core::qualifier_storage::uniform) ? L'_' : L'.');
    }
  }

  return output::operator()(r);
}

sltl::syntax::action_return_t ns::output_hlsl::operator()(const sltl::syntax::variable_declaration& vd, bool is_start)
{
  if(is_start)
  {
    _ss << get_indent(indent_t::current);
    _ss << get_type_name(vd.get_type()) << L' ';

    if(vd._qualifier == core::qualifier_storage::uniform)
    {
      _ss << to_qualifier_prefix_string(vd._qualifier) << L'_';
    }

    _ss << get_variable_name(vd);

    if(vd._qualifier == core::qualifier_storage::in ||
       vd._qualifier == core::qualifier_storage::out)
    {
      const std::wstring semantic_string = ::to_semantic_string(vd._semantic, vd._semantic_index);

      if(!semantic_string.empty())
      {
        _ss << L" : " << semantic_string;
      }
    }
    else if(vd.has_initializer())
    {
      _ss << L" = ";
    }
  }
  else
  {
    _ss << get_terminal_newline();
  }

  return is_start ? syntax::action_return_t::step_in :
                    syntax::action_return_t::step_out;
}

sltl::syntax::action_return_t ns::output_hlsl::operator()(const sltl::syntax::function_definition& fd, bool is_start)
{
  syntax::action_return_t return_val;

  if((fd._name == L"main") && is_start)
  {
    if(_flags.has_flag<output_flags::flag_extra_newlines>())
    {
      _ss << get_newline();
    }

    assert(fd.get_type() == language::type_helper<void>());
    assert(fd.get_params().size() == 0U);

    const syntax::block& function_body = fd.get_body();

    std::wstring block_out_type;
    std::wstring block_out_name;

    _ss << get_indent(indent_t::current);

    // Replace the main function's 'void' return type with the output io_block's type
    if(_block_out)
    {
      block_out_type = ::to_type_string(*_block_out, _stage);
      block_out_name = ::to_qualifier_prefix_string(_block_out->_qualifier);

      _ss << block_out_type;
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

    _ss << L')';
    _ss << get_newline();

    // Output the function body's opening brace
    syntax::action_return_t return_val_body_start = output::operator()(function_body, true);

    // Output a variable declaration statement for the output io_block
    if(_block_out)
    {
      _ss << get_indent(indent_t::current);
      _ss << block_out_type << L' ' << block_out_name;
      _ss << get_terminal_newline();
    }

    auto it = function_body.begin();
    auto it_end = function_body.end();

    if(it != it_end)
    {
      while((*it)->apply_action(*this) && (++it != it_end));
    }

    const bool is_continuing = (it == it_end);

    if(is_continuing)
    {
      // Output a return statement for the output io_block
      if(_block_out)
      {
        if(_flags.has_flag<output_flags::flag_extra_newlines>())
        {
          _ss << get_newline();
        }

        _ss << get_indent(indent_t::current);
        _ss << to_keyword_string(language::id_return) << L' ' << block_out_name;
        _ss << get_terminal_newline();
      }

      // Output the function body's closing brace
      syntax::action_return_t return_val_body_end = output::operator()(function_body, false);

      assert(return_val_body_start == syntax::action_return_t::step_in);
      assert(return_val_body_end   == syntax::action_return_t::step_out);
    }

    // The 'success' return value is 'step_over' as all child nodes have already been traversed
    return_val = (is_continuing ?
      syntax::action_return_t::step_over :
      syntax::action_return_t::stop);
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

const wchar_t* ns::output_hlsl::to_intrinsic_string(sltl::core::intrinsic intrinsic) const
{
  switch(intrinsic)
  {
    case core::intrinsic::dot:
      return L"dot";
    case core::intrinsic::normalize:
      return L"normalize";
    case core::intrinsic::clamp:
      return L"clamp";
    case core::intrinsic::lerp:
      return L"lerp";
    case core::intrinsic::pow:
      return L"pow";
  }

  return nullptr;
}

const wchar_t* ns::output_hlsl::to_intrinsic_operator_string(const sltl::syntax::operator_binary& ob) const
{
  switch(ob._operator_id)
  {
    case language::id_matrix_multiplication:
      return L"mul";
  }

  return nullptr;
}
