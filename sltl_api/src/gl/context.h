#pragma once


namespace sltl
{
namespace api
{
namespace gl
{
  class context final
  {
  public:
    context();

    ~context();

  private:
    struct impl;
    impl* _impl;
  };
}
}
}
