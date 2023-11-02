/*
/
// filename: Rederer.h
// author: Callen Betts
// brief: defines core rendering engine
/
*/

#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>

namespace Graphics
{

  class Renderer
  {

  public:
    Renderer();

    void cleanUp();
    bool createDeviceAndSwapChain();

  private:

    // devices
    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;
    IDXGISwapChain* swapChain;
    // shaders
    ID3D11PixelShader* pixelShader;
    ID3D11VertexShader* vertexShader;
    // blobs
    ID3DBlob* pixelShaderBlob;
    ID3DBlob* vertexShaderBlob;

    // engine
    Engine::GlowEngine* engine;

  };

} // graphics