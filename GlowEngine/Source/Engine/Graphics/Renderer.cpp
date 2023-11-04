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
#include <filesystem>

// initialize the graphics renderer properties
Graphics::Renderer::Renderer(HWND handle)
  :
  windowHandle(handle),
  pixelShader(nullptr),
  vertexShader(nullptr),
  camera(nullptr),
  constantBuffer(nullptr)
{
  engine = EngineInstance::getEngine();
  // init graphics
  float bgCol[4] = { 0,0,0,0 };
  setBackgroundColor(bgCol);
  initGraphics();
  // init camera, which needs access to the renderer
  camera = new Visual::Camera(this);
}

// initialize each part of the graphics pipeline
void Graphics::Renderer::initGraphics()
{
  // create each component of the pipeline starting device and swap chain
  createDeviceAndSwapChain();
  loadShaders();
  createTargetView();
  createViewport();
  setRenderTarget();
  createRasterizer();
  setTopology();
  createConstantBuffer();

  // set the rasterizer to wireframe for testing
  setRasterizerFillMode(D3D11_FILL_WIREFRAME);
}


// test render update for messing with models and such
void Graphics::Renderer::testUpdate()
{
  // use a test model class - hardcode everything to test
  static Entities::EntityList* list;
  static Entities::Entity* entity = new Entities::Entity();
  static Entities::Entity* entity2 = new Entities::Entity();
  static Components::Model* model = new Components::Model();
  static Components::Model* model2 = new Components::Model();
  static Components::Transform* transform = new Components::Transform();
  static Components::Transform* transform2 = new Components::Transform();

  if (!list)
  {
    list = new Entities::EntityList();
    entity->addComponent(model);
    entity->addComponent(transform);
    list->add(entity);
    transform2->setPosition({ 1,0,10 });
    entity2->addComponent(model2);
    entity2->addComponent(transform2);
    list->add(entity2);
  }
  list->update();
}

// free all directX objects from memory
void Graphics::Renderer::cleanup()
{
  constantBuffer->Release();
}

// the beginning of each frame of the render engine
void Graphics::Renderer::beginFrame()
{
  // update the camera
  camera->update();

  // clear the target view for drawing
  clearTargetView();
}

// the end of each frame at the renderer engine
// present the swapchain and draw the objects
void Graphics::Renderer::endFrame()
{
  // present the back buffer to the screen
  swapChain->Present(0, 0);
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
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
  viewport.Width = static_cast<float>(1280);
  viewport.Height = static_cast<float>(720);
  viewport.MinDepth = 0.0f;
  viewport.MaxDepth = 1.0f;
  viewport.TopLeftX = 0.0f;
  viewport.TopLeftY = 0.0f;
  deviceContext->RSSetViewports(1, &viewport);
}

void Graphics::Renderer::setRenderTarget()
{
  // set render target
  deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
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

// create the rasterizer state for defining culling and vertex winding order
// you can also change this to wireframe to see the vertices connect
void Graphics::Renderer::createRasterizer()
{
  // define the rasterizer state description
  ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

  rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME; // Set the fill mode to wireframe
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
