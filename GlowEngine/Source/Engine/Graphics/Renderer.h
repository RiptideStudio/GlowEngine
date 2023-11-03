/*
/
// filename: Rederer.h
// author: Callen Betts
// brief: defines core rendering engine
/
*/

#pragma once

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

namespace Visual
{
  class Camera;
}

namespace Graphics
{

  class Renderer
  {

  public:
    Renderer(HWND handle);

    void initGraphics();
    void testUpdate();

    void createDeviceAndSwapChain();
    void loadShaders();
    void createTargetView();
    void createViewport();

    void clearTargetView();
    void setBackgroundColor(float color[4]);

    void setTopology(D3D_PRIMITIVE_TOPOLOGY topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


  private:

    // window handle
    HWND windowHandle;

    // devices
    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;
    IDXGISwapChain* swapChain;
    ID3D11RenderTargetView* renderTargetView;

    // shaders
    ID3D11PixelShader* pixelShader;
    ID3D11VertexShader* vertexShader;
    // blobs
    ID3DBlob* pixelShaderBlob;
    ID3DBlob* vertexShaderBlob;

    // layout
    ID3D11InputLayout* inputLayout;

    // render target view
    ID3D11Texture2D* backBuffer;

    // background color
    float backgroundColor[4];

    // engine
    Engine::GlowEngine* engine;
    // camera
    Visual::Camera* camera;

  };

} // graphics