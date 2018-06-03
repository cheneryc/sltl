#pragma once

#include <utility>
#include <exception>


namespace sltl
{
namespace core
{
  enum class semantic : unsigned short
  {
    none = 0,
    system,
    position, // For built-in behaviour use semantic_system::position
    depth,    // For built-in behaviour use semantic_system::depth
    normal,
    texcoord,
    colour,   // For built-in behaviour use semantic_system::target
    material,
    transform,
    camera,
    light,
    user,
    count
  };

  // semantic_system::position -  gl_Position/gl_FragCoord or SV_Position
  // semantic_system::depth -     gl_FragDepth or SV_Depth
  // semantic_system::target -    (layout(location=N) out your_var_name;) or SV_Target
  enum class semantic_system : unsigned char
  {
    none = 0,
    position = static_cast<std::underlying_type<semantic>::type>(semantic::count),
    depth,
    target,
    count
  };

  enum class semantic_transform : unsigned char
  {
    none = 0,
    model = static_cast<std::underlying_type<semantic_system>::type>(semantic_system::count),
    modelview,
    modelviewproj,
    modelviewproj_inverse,
    modelviewproj_inversetrans,
    view,
    viewproj,
    proj,
    normal, // mat3x3 - modelviewproj_inversetrans
    count
  };

  typedef unsigned short semantic_index_t;

  template<typename T>
  struct semantic_index_limits;

  template<>
  struct semantic_index_limits<semantic_system>
  {
    constexpr static semantic_index_t max()
    {
      return 15U;
    }
  };

  class semantic_pair
  {
  public:
    semantic_pair(semantic s = semantic::none, semantic_index_t index = 0U) : _semantic(s), _index(index) {}
    semantic_pair(const semantic_pair& pair) : _semantic(pair._semantic), _index(pair._index) {}

    // Non-assignable
    semantic_pair& operator=(semantic_pair&&) = delete;
    semantic_pair& operator=(const semantic_pair&) = delete;

    const semantic _semantic;
    const semantic_index_t _index;

    static const semantic_pair none;
  };

  namespace detail
  {
    constexpr auto to_underlying_value(semantic_system s) -> std::underlying_type<semantic_system>::type
    {
      return (static_cast<std::underlying_type<semantic_system>::type>(s) -
              static_cast<std::underlying_type<semantic>::type>(semantic::count)) + 1;
    }

    constexpr auto to_underlying_value(semantic_transform s) -> std::underlying_type<semantic_transform>::type
    {
      return (static_cast<std::underlying_type<semantic_transform>::type>(s) -
              static_cast<std::underlying_type<semantic_system>::type>(semantic_system::count)) + 1;
    }

    constexpr semantic_index_t to_semantic_index(semantic_system s, semantic_index_t n)
    {
      // Each system semantic is allocated 16 indices (4 bits)
      // position (1) ->  0 - 15
      // depth (2)    -> 16 - 31
      // target (3)   -> 32 - 47

      return s != semantic_system::none ? ((to_underlying_value(s) - 1U) << 4) + n : throw std::exception();//TODO: exception type and message
    }

    constexpr semantic_index_t to_semantic_index(semantic_transform s)
    {
      return s != semantic_transform::none ? (to_underlying_value(s) - 1) : throw std::exception();//TODO: exception type and message
    }

    inline std::pair<semantic_system, semantic_index_t> to_semantic_system_pair(semantic_index_t n)
    {
      const div_t div_result = div(n, 16);

      return {
        static_cast<semantic_system>(div_result.quot + static_cast<std::underlying_type<semantic>::type>(semantic::count)),
        static_cast<semantic_index_t>(div_result.rem)
      };
    }
  }
}
}
