#include "shader_dx.h"

#include <d3dcompiler.h>

#include <wrl/client.h>

#include <stdexcept>

#include <cassert>


namespace
{
  const char* get_shader_target(sltl::api::shader_stage stage)
  {
    switch(stage)
    {
      case sltl::api::shader_stage::vertex:
        return "vs_4_0";
      case sltl::api::shader_stage::fragment:
        return "ps_4_0";
      default:
        assert((stage == sltl::api::shader_stage::vertex) || (stage == sltl::api::shader_stage::fragment));
    }

    return nullptr;
  }

  namespace ns = sltl::api::dx;
}

sltl::api::detail::blob ns::shader::compile(const char* shader_src, sltl::api::shader_stage stage)
{
  Microsoft::WRL::ComPtr<::ID3DBlob> blobCode;
  Microsoft::WRL::ComPtr<::ID3DBlob> blobErrors;

  const char* const shader_target = get_shader_target(stage);

  const UINT shader_flags =
    D3DCOMPILE_ENABLE_STRICTNESS |
    D3DCOMPILE_WARNINGS_ARE_ERRORS;

  const HRESULT hr = ::D3DCompile(shader_src, strlen(shader_src), nullptr, nullptr, nullptr, "main", shader_target, shader_flags, 0U, &blobCode, &blobErrors);

  if(FAILED(hr))
  {
    throw std::runtime_error(static_cast<const char*>(blobErrors->GetBufferPointer()));
  }

  return {
    blobCode->GetBufferPointer(),
    blobCode->GetBufferSize()
  };
}
