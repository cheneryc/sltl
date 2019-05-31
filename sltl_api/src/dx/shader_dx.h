#pragma once

#include "../shader_stage.h"

#include "../detail/blob.h"
#include "../detail/blob_view.h"


namespace sltl
{
namespace api
{
namespace dx
{
  class shader final
  {
  public:
    static detail::blob compile(const char* shader_src, shader_stage stage);

    shader(detail::blob_view shader_blob_view, shader_stage stage)
    {
      //TODO: implement
    }
  };

  //TODO: move shader_program class into its own .h/cpp
  class shader_program final
  {
  public:
    shader_program(const shader& shader_vertex, const shader& shader_fragment)
    {
      //TODO: implement
    }

    ~shader_program()
    {
      //TODO: implement
    }
  };
}
}
}
