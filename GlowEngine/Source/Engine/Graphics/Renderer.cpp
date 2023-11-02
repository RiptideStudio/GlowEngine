/*
/
// filename: stub.cpp
// author: Callen Betts
// brief: implements stub.h
/
*/

#include "stdafx.h"
#include "Renderer.h"

// initialize the graphics renderer properties
Graphics::Renderer::Renderer()
  :
  pixelShader(nullptr),
  vertexShader(nullptr),
  pixelShaderBlob(nullptr),
  vertexShaderBlob(nullptr),
  device(nullptr),
  deviceContext(nullptr),
  swapChain(nullptr)
{
  engine = EngineInstance::getEngine();
}

// initialize the d3d device and swap chain
bool Graphics::Renderer::createDeviceAndSwapChain()
{
  // initialize the device
  HRESULT hr = D3D11CreateDevice(
    nullptr,
    D3D_DRIVER_TYPE_HARDWARE,
    nullptr,
    0,
    nullptr,
    D3D_FEATURE_LEVEL_11_0,
    0,
    &device,
    nullptr,
    &deviceContext
  );

  // failed to create device
  if (!hr)
  {
    return false;
  }

  // success
  return true;
}

// cleanup and release the various directX objects
void Graphics::Renderer::cleanUp()
{

}