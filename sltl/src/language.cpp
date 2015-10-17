#include "language.h"

#include <sstream>

#include <cassert>


namespace
{
  namespace ns = sltl::language;
}

std::wstring ns::to_type_string(const type& t)
{
  std::wstringstream ss;

  if(t.get_dimension_count() > 1)
  {
    switch(t._id)
    {
    case id_float:
      ss << L"mat";
      break;
    case id_double:
      ss << L"dmat";
      break;
    default:
      assert((t._id == id_float) || (t._id == id_double));
    }

    const auto it_end = t.crend();

    // use reverse iterators as glsl matrices are column-major
    for(auto it = t.crbegin(); it != it_end; ++it)
    {
      ss << *it;

      if(std::next(it) != it_end)
      {
        ss << L'x';
      }
    }
  }
  else
  {
    assert(t.get_dimension_count() == 1U);

    if(t.front() > 1)
    {
      switch(t._id)
      {
      case id_float:
        ss << L"vec";
        break;
      case id_double:
        ss << L"dvec";
        break;
      case id_int:
        ss << L"ivec";
        break;
      case id_uint:
        ss << L"uvec";
        break;
      case id_bool:
        ss << L"bvec";
        break;
      default:
        assert((t._id != id_unknown) && (t._id != id_void));
      }

      ss << t.front();
    }
    else
    {
      assert(t.front() != 0);

      switch(t._id)
      {
      case id_void:
        ss << L"void";
        break;
      case id_float:
        ss << L"float";
        break;
      case id_double:
        ss << L"double";
        break;
      case id_int:
        ss << L"int";
        break;
      case id_uint:
        ss << L"unsigned int";
        break;
      case id_bool:
        ss << L"bool";
        break;
      default:
        assert(t._id != id_unknown);
      }
    }
  }

  return ss.str();
}

std::wstring ns::to_type_prefix_string(const type& t)
{
  // Variable names can't begin with a numeral so we need a type specific prefix
  std::wstring prefix_string;

  if(t.get_dimension_count() > 1)
  {
    assert((t._id == id_float) ||
           (t._id == id_double));

    prefix_string = L'm';
  }
  else
  {
    assert(t.get_dimension_count() == 1U);

    if(t.front() > 1)
    {
      assert(t._id != id_void);
      assert(t._id != id_unknown);

      prefix_string = L'v';
    }
    else
    {
      assert(t.front() != 0);

      switch(t._id)
      {
      case id_float:
        prefix_string = L'f';
        break;
      case id_double:
        prefix_string = L'd';
        break;
      case id_int:
        prefix_string = L'i';
        break;
      case id_uint:
        prefix_string = L'u';
        break;
      case id_bool:
        prefix_string = L'b';
        break;
      default:
        assert((t._id != id_unknown) && (t._id != id_void));
      }
    }
  }

  return std::move(prefix_string);
}

const wchar_t* ns::to_operator_string(operator_id id)
{
  switch(id)
  {
  case id_addition:
    return L"+";
   case id_subtraction:
    return L"-";
   case id_multiplication:
    return L"*";
   case id_division:
    return L"/";
  }

  return nullptr;
}

const wchar_t* ns::to_assignment_operator_string(assignment_operator_id id)
{
  switch(id)
  {
  case id_assignment:
    return L"=";
  case id_assignment_addition:
    return L"+=";
   case id_assignment_subtraction:
    return L"-=";
   case id_assignment_multiplication:
    return L"*=";
   case id_assignment_division:
    return L"/=";
  }

  return nullptr;
}

const wchar_t* ns::to_conditional_string(conditional_id id)
{
  switch(id)
  {
  case id_if:
    return L"if";
  case id_else:
    return L"else";
  case id_else_if:
    return L"else if";
  }

  return nullptr;
}

const wchar_t* ns::to_keyword_string(keyword_id id)
{
  switch(id)
  {
  case id_return:
    return L"return";
  }

  return nullptr;
}

const wchar_t* ns::to_qualifier_string(sltl::core::qualifier_storage id)
{
  switch(id)
  {
  case sltl::core::qualifier_storage::in:
    return L"in";
  case sltl::core::qualifier_storage::out:
    return L"out";
  case sltl::core::qualifier_storage::uniform:
    return L"uniform";
  }

  return nullptr;
}

const wchar_t* ns::to_qualifier_prefix_string(sltl::core::qualifier_storage id)
{
  switch(id)
  {
  case sltl::core::qualifier_storage::in:
    return L"i";
    break;
  case sltl::core::qualifier_storage::out:
    return L"o";
    break;
  case sltl::core::qualifier_storage::uniform:
    return L"u";
    break;
  }

  return nullptr;
}
