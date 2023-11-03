/*
/
// filename: Renderer.cpp
// author: Callen Betts
// brief: implements the rendering engine and pipeline
/
*/

#include "stdafx.h"
#include "Renderer.h"
#include <filesystem>
#include "Engine/GlowEngine.h"
#include "Camera/Camera.h"

// initialize the graphics renderer properties
Graphics::Renderer::Renderer(HWND handle)
  :
  windowHandle(handle),
  pixelShader(nullptr),
  vertexShader(nullptr),
  camera(nullptr)
{
  engine = EngineInstance::getEngine();
  // init graphics
  float bgCol[4] = { 0,0,0,0 };
  setBackgroundColor(bgCol);
  initGraphics();
  // init camera, which needs window handle (maybe)
  camera = new Visual::Camera(handle);
}

// initialize each part of the graphics pipeline
void Graphics::Renderer::initGraphics()
{
  createDeviceAndSwapChain();
  loadShaders();
  createTargetView();
  createViewport();
  createRasterizer();
  setTopology();
}

void Graphics::Renderer::testUpdate()
{

  // use a test model class - hardcode everything to test
  camera->update();
  static Components::Model* model = new Components::Model();
  static Components::Transform* transform = new Components::Transform();

  transform->recalculateMatrix();
  Matrix worldMatrix = transform->getTransformMatrix();
  Matrix viewMatrix = camera->getViewMatrix();
  Matrix perspectiveMatrix = camera->getPerspecitveMatrix();

  // Define a structure that matches the shader constant buffer layout
  struct ConstantBufferType 
  {
    DirectX::XMMATRIX world;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
  };

  // Create a constant buffer
  ID3D11Buffer* constantBuffer = nullptr;
  D3D11_BUFFER_DESC constantBufferDesc;
  ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

  // Set up the description of the constant buffer
  constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  constantBufferDesc.ByteWidth = sizeof(ConstantBufferType);
  constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  constantBufferDesc.CPUAccessFlags = 0;
  constantBufferDesc.MiscFlags = 0;
  constantBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class
  HRESULT result = device->CreateBuffer(&constantBufferDesc, NULL, &constantBuffer);
  if (FAILED(result)) 
  {
    Logger::error("Failed to create constant buffer");
  }

  int frames = engine->getTotalFrames();
  if (frames % 6000 == 0)
  {
    setRasterizerFillMode(D3D11_FILL_WIREFRAME);
  }
  if (frames % 12000 == 0)
  {
    setRasterizerFillMode(D3D11_FILL_SOLID);
  }

  // Create vertex buffer
  ID3D11Buffer* vertexBuffer = nullptr;
  D3D11_BUFFER_DESC vertexBufferDesc = {};
  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof(Vertex) * model->getVerties().size();
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;

  D3D11_SUBRESOURCE_DATA vertexData = {};
  vertexData.pSysMem = model->getVerties().data();
  device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

  ID3D11Buffer* indexBuffer = nullptr;
  D3D11_BUFFER_DESC indexBufferDesc = {};
  indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  indexBufferDesc.ByteWidth = sizeof(unsigned short) * model->getIndices().size();
  indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.CPUAccessFlags = 0;

  D3D11_SUBRESOURCE_DATA indexData = {};
  indexData.pSysMem = model->getIndices().data();
  device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

  // set the 
  UINT stride = sizeof(Vertex);
  UINT offset = 0;
  deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
  deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
  
  // Update the constant buffer with the matrices
  ConstantBufferType cbData;
  cbData.world = DirectX::XMMatrixTranspose(worldMatrix);
  cbData.view = DirectX::XMMatrixTranspose(viewMatrix);
  cbData.projection = DirectX::XMMatrixTranspose(perspectiveMatrix);

  // Note: DirectX::XMMATRIX is assuming you're using DirectXMath for your matrix math
  if (!constantBuffer)
  {
    Logger::error("Bad CB buffer");
  }
  else
  {
    deviceContext->UpdateSubresource(constantBuffer, 0, nullptr, &cbData, 0, 0);
  }

  // Bind the constant buffer to the vertex shader
  deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);

  // clear the target view for drawing
  clearTargetView();

  // Set render target
  deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);

  // Draw the triangle
  deviceContext->DrawIndexed(model->getIndices().size(), 0, 0);

  // Present the back buffer to the screen
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
