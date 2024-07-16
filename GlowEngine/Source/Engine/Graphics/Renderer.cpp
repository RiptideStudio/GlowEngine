/*
/
// filename: Renderer.cpp
// author: Callen Betts
// brief: implements the rendering engine and pipeline
/
*/

#include "stdafx.h"
#include "Renderer.h"
#include "Engine/GlowEngine.h"
#include "Camera/Camera.h"
#include "Engine/Entity/Entity.h"
#include "Engine/Entity/EntityList/EntityList.h"
#include "Engine/Graphics/Textures/Texture.h"
#include "Engine/Graphics/Window/Window.h"
#include "Engine/Graphics/Textures/stb_image.h"
#include "UI/GlowGui.h"
#include <filesystem>

// define the amount of max lights we can have
#define MAXLIGHTS 8
static PointLight* pointLightsArray[MAXLIGHTS];

// initialize the graphics renderer properties
Graphics::Renderer::Renderer(HWND handle)
  :
  windowHandle(handle),
  pixelShader(nullptr),
  vertexShader(nullptr),
  camera(nullptr),
  constantBuffer(nullptr),
  sampler(nullptr),
  glowGui(nullptr),
  lights(0)
{
  // engine
  engine = EngineInstance::getEngine();
  // window
  window = engine->getWindow();
  // graphics
  float bgCol[4] = { 0,0,0.025,1 };
  setBackgroundColor(bgCol);
  initGraphics();
  // camera
  camera = new Visual::Camera(this);
}

// initialize each part of the graphics pipeline
void Graphics::Renderer::initGraphics()
{
  // create each component of the pipeline starting device and swap chain
  createDeviceAndSwapChain();
  createTargetView();
  loadShaders();
  createBlendState();
  createViewport();
  setRenderTarget();
  createRasterizer();
  createDepthStencil();
  setTopology();
  createConstantBuffer();
  createSamplerState();
  createLightBuffer();
  createImGuiSystem();

  // set the rasterizer to wireframe for testing
  // setRasterizerFillMode(D3D11_FILL_WIREFRAME);
}

// free all directX objects from memory
void Graphics::Renderer::cleanup()
{
  constantBuffer->Release();
}

// the beginning of each frame of the render engine
void Graphics::Renderer::beginFrame()
{
  // update the camera matrix
  camera->update();

  // clear the target view for drawing
  clearTargetView();

  // set the render target and clear depth buffer
  setRenderTarget();

  // start imGui frame
  glowGui->beginUpdate();

  // Update the GPU constant buffer
  if (*pointLightsArray)
  {
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    deviceContext->Map(pointLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    memcpy(mappedResource.pData, *pointLightsArray, sizeof(PointLightBuffer) * MAXLIGHTS);
    deviceContext->Unmap(pointLightBuffer, 0);
  }

  // bind the constant buffer to the shader
  deviceContext->PSSetConstantBuffers(0, 1, &pointLightBuffer);

  // bind the texture sampler
  deviceContext->PSSetSamplers(0, 1, &sampler);
}

// the end of each frame at the renderer engine
// present the swapchain and draw the objects
void Graphics::Renderer::endFrame()
{
  // end ImGui draw
  glowGui->endUpdate();

  // present the back buffer to the screen
  swapChain->Present(1, 0);
}

// initialize the d3d device, context and swap chain
void Graphics::Renderer::createDeviceAndSwapChain()
{
  // Describe our SwapChain
  DXGI_SWAP_CHAIN_DESC swapChainDesc;
  ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

  swapChainDesc.BufferCount = 1;
  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.OutputWindow = windowHandle;
  swapChainDesc.SampleDesc.Count = 1;
  swapChainDesc.Windowed = TRUE;

  // Create a device, device context, and swap chain using the information in the swapChainDesc structure
  D3D_FEATURE_LEVEL featureLevel;
  HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
    D3D_DRIVER_TYPE_HARDWARE,
    NULL,
    0,
    NULL,
    0,
    D3D11_SDK_VERSION,
    &swapChainDesc,
    &swapChain,
    &device,
    &featureLevel,
    &deviceContext
  );

  // failed to create
  if (FAILED(hr))
  {
    Logger::error("Failed to create swap chain and device");
  }
}

// load compiled shader files and create the needed shaders
// there are three main shaders: pixel, pixelTex, vertex
void Graphics::Renderer::loadShaders()
{
  // Get the path of the current executable
  wchar_t buffer[MAX_PATH];
  GetModuleFileNameW(nullptr, buffer, MAX_PATH);
  std::wstring executablePath = buffer;

  // Get the parent directory of the executable directory
  std::filesystem::path executableDir = executablePath;
  std::filesystem::path shaderDirectory = executableDir.parent_path();

  // Construct the full file paths for the shader files
  std::wstring vertexShaderPath = (shaderDirectory / L"VertexShader.cso").wstring();
  std::wstring pixelShaderPath = (shaderDirectory / L"PixelShader.cso").wstring();
  std::wstring pixelTexShaderPath = (shaderDirectory / L"PixelShaderTex.cso").wstring();

  // Load the vertex shader
  HRESULT hr = D3DReadFileToBlob(vertexShaderPath.c_str(), &vertexShaderBlob);
  if (FAILED(hr))
  {
    Logger::error("Failed to create vertex blob");
  }
  hr = device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);
  if (FAILED(hr))
  {
    Logger::error("Failed to create vertex shader");
  }

  // Load the pixel shader
  hr = D3DReadFileToBlob(pixelShaderPath.c_str(), &pixelShaderBlob);
  if (FAILED(hr))
  {
    Logger::error("Failed to create pixel blob");
  }
  hr = device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader);
  if (FAILED(hr))
  {
    Logger::error("Failed to create pixel shader");
  }

  // Set the shaders to the device context
  deviceContext->VSSetShader(vertexShader, nullptr, 0);
  deviceContext->PSSetShader(pixelShader, nullptr, 0);

  // Define the input layout
  D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = 
  {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };

  // Create the input layout
  hr = device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);
  if (FAILED(hr))
  {
    Logger::error("Failed to create input layout");
  }

  // Set the input layout to the device context 
  deviceContext->IASetInputLayout(inputLayout);

}

// create the target view fo the renderer
void Graphics::Renderer::createTargetView()
{
  // Create render target view
  ID3D11Texture2D* backBuffer = nullptr;
  swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
  if (backBuffer)
  {
    device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    backBuffer->Release();
  }
}

// create the viewport for the renderer with given width and height
// this can be changed through the engine
void Graphics::Renderer::createViewport()
{
  // Create and set the viewport
  D3D11_VIEWPORT viewport;
  viewport.Width = static_cast<float>(window->getWidth());
  viewport.Height = static_cast<float>(window->getHeight());
  viewport.MinDepth = 0.0f;
  viewport.MaxDepth = 1.0f;
  viewport.TopLeftX = 0.0f;
  viewport.TopLeftY = 0.0f;
  deviceContext->RSSetViewports(1, &viewport);
}

// set the render target
void Graphics::Renderer::setRenderTarget()
{
  // clear depth buffer
  deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

  // set render target
  deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
}

// create a blend state so we can have alpha
void Graphics::Renderer::createBlendState()
{
  D3D11_BLEND_DESC blendDesc = { 0 };
  blendDesc.RenderTarget[0].BlendEnable = TRUE;
  blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

  ID3D11BlendState* pBlendState = NULL;
  device->CreateBlendState(&blendDesc, &pBlendState);
  deviceContext->OMSetBlendState(pBlendState, NULL, 0xffffffff);
}

// create a depth stencil to define stencil tests and depth ordering
void Graphics::Renderer::createDepthStencil()
{
  D3D11_TEXTURE2D_DESC depthStencilDesc;

  depthStencilDesc.Width = window->getWidth(); // Your render target width
  depthStencilDesc.Height = window->getHeight(); // Your render target height
  depthStencilDesc.MipLevels = 1;
  depthStencilDesc.ArraySize = 1;
  depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depthStencilDesc.SampleDesc.Count = 1; // Change if using MSAA
  depthStencilDesc.SampleDesc.Quality = 0; // Change if using MSAA
  depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
  depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  depthStencilDesc.CPUAccessFlags = 0;
  depthStencilDesc.MiscFlags = 0;

  HRESULT hr = device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
  if (FAILED(hr))
  {
    Logger::error("Failed to create depth stencil texture");
  }

  // Create the depth stencil view
  D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
  ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
  depthStencilViewDesc.Format = depthStencilDesc.Format;
  depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  depthStencilViewDesc.Texture2D.MipSlice = 0;

  hr = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);

  if (FAILED(hr))
  {
    Logger::error("Failed to create depth stencil view");
  }

  // describe the depth stencil state
  D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
  ZeroMemory(&depthStencilStateDesc, sizeof(depthStencilStateDesc));

  depthStencilStateDesc.DepthEnable = true;
  depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

  depthStencilStateDesc.StencilEnable = true;
  depthStencilStateDesc.StencilReadMask = 0xFF;
  depthStencilStateDesc.StencilWriteMask = 0xFF;

  depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  hr = device->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilState);
  if (FAILED(hr))
  {
  }

  deviceContext->OMSetDepthStencilState(depthStencilState, 1);
}

// setup a light buffer
void Graphics::Renderer::createLightBuffer()
{
  // create a constant buffer
  D3D11_BUFFER_DESC lightBufferDesc = {};
  lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  lightBufferDesc.ByteWidth = sizeof(LightBuffer);
  lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  lightBufferDesc.MiscFlags = 0;
  lightBufferDesc.StructureByteStride = 0;

  HRESULT result = device->CreateBuffer(&lightBufferDesc, nullptr, &lightBuffer);
  if (FAILED(result)) {
    Logger::error("Failed to create light buff");
  }

  ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
  HRESULT mapResult = deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(mapResult)) 
  {
  }

  // create light description
  D3D11_BUFFER_DESC cbd;
  ZeroMemory(&cbd, sizeof(D3D11_BUFFER_DESC));
  cbd.Usage = D3D11_USAGE_DYNAMIC;
  cbd.ByteWidth = sizeof(PointLightBuffer)*MAXLIGHTS;
  cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

  device->CreateBuffer(&cbd, nullptr, &pointLightBuffer);
}

// create a sampler state for sampling textures
// the address mode should be wrap by default
void Graphics::Renderer::createSamplerState()
{
  D3D11_SAMPLER_DESC sampDesc;
  ZeroMemory(&sampDesc, sizeof(sampDesc));
  sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
  device->CreateSamplerState(&sampDesc, &sampler);
}

void Graphics::Renderer::createRenderTexture()
{

}

// create a shadow map
void Graphics::Renderer::createShadowMap()
{
  D3D11_TEXTURE2D_DESC depthTexDesc;
  ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
  depthTexDesc.Width = window->getWidth();
  depthTexDesc.Height = window->getHeight();
  depthTexDesc.MipLevels = 1;
  depthTexDesc.ArraySize = 1;
  depthTexDesc.Format = DXGI_FORMAT_R32_TYPELESS; // Use a typeless format for more flexibility
  depthTexDesc.SampleDesc.Count = 1;
  depthTexDesc.SampleDesc.Quality = 0;
  depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
  depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE; // Bind as depth stencil and shader resource
  depthTexDesc.CPUAccessFlags = 0;
  depthTexDesc.MiscFlags = 0;

  device->CreateTexture2D(&depthTexDesc, NULL, &shadowMap);

  D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
  ZeroMemory(&dsvDesc, sizeof(dsvDesc));
  dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // Use a format that includes a depth component
  dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  dsvDesc.Texture2D.MipSlice = 0;

  device->CreateDepthStencilView(shadowMap, &dsvDesc, &shadowMapDSV);

  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
  ZeroMemory(&srvDesc, sizeof(srvDesc));
  srvDesc.Format = DXGI_FORMAT_R32_FLOAT; // Match the format for the shader resource
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MostDetailedMip = 0;
  srvDesc.Texture2D.MipLevels = 1;

  ID3D11ShaderResourceView* shadowMapSRV;
  device->CreateShaderResourceView(shadowMap, &srvDesc, &shadowMapSRV);

  D3D11_VIEWPORT shadowViewport;
  shadowViewport.Width = static_cast<float>(window->getWidth());
  shadowViewport.Height = static_cast<float>(window->getHeight());
  shadowViewport.MinDepth = 0.0f;
  shadowViewport.MaxDepth = 1.0f;
  shadowViewport.TopLeftX = 0;
  shadowViewport.TopLeftY = 0;

  deviceContext->RSSetViewports(1, &shadowViewport);
}

// create a constant buffer that should not be changed
void Graphics::Renderer::createConstantBuffer()
{
  // create a constant buffer
  ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

  // set up the description of the constant buffer
  constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  constantBufferDesc.ByteWidth = sizeof(cbPerObject);
  constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  constantBufferDesc.CPUAccessFlags = 0;
  constantBufferDesc.MiscFlags = 0;
  constantBufferDesc.StructureByteStride = 0;

  // create the buffer itself
  device->CreateBuffer(&constantBufferDesc, NULL, &constantBuffer);
}

// bind the subresource and constant buffer to the renderer
void Graphics::Renderer::updateConstantBuffer()
{
  // update device context sub resource
  deviceContext->UpdateSubresource(constantBuffer, 0, nullptr, &cbData, 0, 0);

  // bind the constant buffer to the vertex shader
  deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);
}

// given the transform matrix, update the cb object's world matrix
void Graphics::Renderer::updateConstantBufferWorldMatrix(Matrix transformMatrix)
{
  cbData.world = DirectX::XMMatrixTranspose(transformMatrix);
}

// update the view and perspective matrices in the constant buffer using camera
void Graphics::Renderer::updateConstantBufferCameraMatrices()
{
  cbData.view = DirectX::XMMatrixTranspose(camera->getViewMatrix());
  cbData.projection = DirectX::XMMatrixTranspose(camera->getPerspecitveMatrix());
}

void Graphics::Renderer::toggleFullscreen(bool val)
{
  fullscreen = !fullscreen;
  swapChain->SetFullscreenState(val, nullptr);
}

// set the texture resource to nullptr which means no texture
void Graphics::Renderer::unBindTexture()
{
  ID3D11ShaderResourceView* nullSRV[] = { nullptr };
  deviceContext->PSSetShaderResources(0, 1, nullSRV);
}

// create the rasterizer state for defining culling and vertex winding order
// you can also change this to wireframe to see the vertices connect
void Graphics::Renderer::createRasterizer()
{
  // define the rasterizer state description
  ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

  rasterizerDesc.FillMode = D3D11_FILL_SOLID; // Set the fill mode to wireframe
  rasterizerDesc.CullMode = D3D11_CULL_NONE;      // No culling to see all the lines
  rasterizerDesc.FrontCounterClockwise = false;   // Define the front-facing side
  rasterizerDesc.DepthClipEnable = true;          // Enable depth clipping

  // create the rasterizer state
  HRESULT result = device->CreateRasterizerState(&rasterizerDesc, &wireframeRasterizerState);
  if (FAILED(result))
  {
    Logger::error("Failed to create rasterizer state");
  }

  // bind the rasterizer state to the pipeline
  deviceContext->RSSetState(wireframeRasterizerState);
}

// set the rasterizer's fill mode with a default of fill all
void Graphics::Renderer::setRasterizerFillMode(D3D11_FILL_MODE fillMode)
{
  // set the fill mode
  rasterizerDesc.FillMode = fillMode;
  // recreate the rasterizer state - this will need to change once we finish setup of main rendering pipeline
  device->CreateRasterizerState(&rasterizerDesc, &wireframeRasterizerState);
  // re-bind the rasterizer state
  deviceContext->RSSetState(wireframeRasterizerState);
}

// clear the target view - this should be done every frame
void Graphics::Renderer::clearTargetView()
{
  deviceContext->ClearRenderTargetView(renderTargetView, backgroundColor);
}

// set the background color 
void Graphics::Renderer::setBackgroundColor(float color[4])
{
  backgroundColor[0] = color[0];
  backgroundColor[1] = color[1];
  backgroundColor[2] = color[2];
  backgroundColor[3] = color[3];
}

// set the renderer's topology
void Graphics::Renderer::setTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
  deviceContext->IASetPrimitiveTopology(topology);
}

// add a new active point light
void Graphics::Renderer::addPointLight(PointLight* buff)
{
  if (lights > 7)
    return;

  pointLightsArray[lights] = buff;
  lights++;
}

// add a new active point light
void Graphics::Renderer::updatePointLight(PointLight* buff)
{
  for (int i = 0; i < lights; ++i)
  {
    if (pointLightsArray[i] == buff)
    {
      pointLightsArray[i] = buff;
    }
  }
}

// get the device
ID3D11Device* Graphics::Renderer::getDevice()
{
  return device;
}

// get the device context
ID3D11DeviceContext* Graphics::Renderer::getDeviceContext()
{
  return deviceContext;
}

void Graphics::Renderer::createImGuiSystem()
{
  glowGui = new Graphics::GlowGui(windowHandle,device,deviceContext,this);
}

ID3D11Texture2D* Graphics::Renderer::getBackBuffer()
{
  return backBuffer;
}
