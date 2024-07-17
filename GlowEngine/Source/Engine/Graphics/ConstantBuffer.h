/*
/
// filename: ConstantBuffer.h
// author: Callen Betts
// brief: defines ConstantBuffer class
/
*/

#pragma once

namespace Graphics
{

  // whether this buffer is bound to a pixel or vertex shader
  enum class ShaderType
  {
    Pixel,
    Vertex
  };

  template <typename T>
  class ConstantBuffer
  {

  public:

    // create the buffer
    ConstantBuffer(ID3D11Device* device) : buffer(nullptr)
    {
      D3D11_BUFFER_DESC desc = {};
      desc.Usage = D3D11_USAGE_DYNAMIC;
      desc.ByteWidth = sizeof(T);
      desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

      HRESULT hr = device->CreateBuffer(&desc, nullptr, &buffer);

      if (FAILED(hr))
      {
        throw std::runtime_error("Failed to create constant buffer");
      }
    }

    // delete the buffer and release it
    ~ConstantBuffer()
    {
      if (buffer)
        buffer->Release();
    }

    // map the buffer to the context
    void update(ID3D11DeviceContext* context, const T& data)
    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      HRESULT hr = context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

      if (FAILED(hr))
      {
        throw std::runtime_error("Failed to map constant buffer");
      }

      memcpy(mappedResource.pData, &data, sizeof(T));
      context->Unmap(buffer, 0);
    }

    // bind it to a shader slot
    void bind(ID3D11DeviceContext* context, UINT slot, ShaderType shaderType)
    {
      switch (shaderType)
      {
      case Graphics::ShaderType::Pixel:
        context->PSSetConstantBuffers(slot, 1, &buffer);
        break;

      case Graphics::ShaderType::Vertex:
        context->VSSetConstantBuffers(slot, 1, &buffer);
        break;
      }
    }
   
  private:

    ID3D11Buffer* buffer;

  };
}
