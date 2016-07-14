#pragma once


namespace sltl
{
namespace core
{
  enum class semantic : unsigned short
  {
    none,
    position,
    depth,
    normal,
    texcoord,
    target,
    user
  };

  typedef unsigned short semantic_index_t;

  class semantic_pair
  {
  public:
    semantic_pair(semantic s = semantic::none, semantic_index_t index = 0U) : _semantic(s), _index(index) {}

    // Non-assignable
    semantic_pair& operator=(semantic_pair&&) = delete;
    semantic_pair& operator=(const semantic_pair&) = delete;

    const semantic _semantic;
    const semantic_index_t _index;

    static const semantic_pair none;
  };
}
}
