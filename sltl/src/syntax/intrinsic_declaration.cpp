#include "intrinsic_declaration.h"


namespace
{
  namespace ns = sltl::syntax;
}

const wchar_t* ns::to_intrinsic_string(sltl::core::intrinsic id)
{
  // _bi_* -> 'built-in' functions

  switch(id)
  {
  case sltl::core::intrinsic::dot:
    return L"_bi_dot";
  case sltl::core::intrinsic::normalize:
    return L"_bi_normalize";
  case sltl::core::intrinsic::clamp:
    return L"_bi_clamp";
  case sltl::core::intrinsic::lerp:
    return L"_bi_lerp";
  case sltl::core::intrinsic::pow:
    return L"_bi_pow";
  }

  return nullptr;
}
