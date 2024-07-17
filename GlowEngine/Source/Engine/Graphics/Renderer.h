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
#include "Engine/Graphics/Lighting/LightBuffer.h"
#include "ConstantBuffer.h"

namespace Visual
{
  class Camera;
}

namespace Graphics
{
  class GlowGui;
  class Window;
}

namespace Graphics
{

  // define a constant buffer to be sent to the vertex shader
  struct cbPerObject
  {
    DirectX::XMMATRIX world;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
  };

  class Renderer
  {

  public:
    Renderer(HWND handle);

    void initGraphics();
    void cleanup(); // release all of the d3d objects

    // frame updates
    void beginFrame();
    void endFrame();
    
    void createDeviceAndSwapChain();
    void loadShaders();
    void createTargetView();
    void createViewport();
    void setRenderTarget();
    void createBlendState();
    void createDepthStencil();
    void createLightBuffer();
    void createSamplerState();
    void createRenderTexture();

    // shadow mapping
    void createShadowMap();

    // constant buffer - this needs to be set using the model's properties
    void createConstantBuffer();
    // the constant buffer needs to be updated whenever a model is being rendered
    void updateConstantBuffer();
    // update the transform matrix within the constant buffer
    void updateConstantBufferWorldMatrix(Matrix world);
    // update the perspective and camera view matrix
    void updateConstantBufferCameraMatrices();

    // set the target color
    void drawSetColor(const Color& color);

    // toggle fullscreen
    void toggleFullscreen(bool val);
    bool isFullscreen() { return fullscreen; }

    // unbind a texture 
    void unBindTexture();

    // rasterizer states and helpers
    void createRasterizer();
    void setRasterizerFillMode(D3D11_FILL_MODE fillMode = D3D11_FILL_WIREFRAME);

    // clear the target view with a background colour
    void clearTargetView();
    void setBackgroundColor(float color[4]);

    // set the topology with a default of trianglelist
    void setTopology(D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // manage point lights
    void addPointLight(PointLight* light);
    void updatePointLight(PointLight* light);

    // get the directX devices for draw calls
    ID3D11Device* getDevice();
    ID3D11DeviceContext* getDeviceContext();

    // create the ImGui system
    void createImGuiSystem();

    // get the back buffer
    ID3D11Texture2D* getBackBuffer();

    // get the camera
    Visual::Camera* getCamera() { return camera; }

  private:

    // window handle
    HWND windowHandle;

    // ImGui system
    Graphics::GlowGui* glowGui;

    // devices
    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;
    IDXGISwapChain* swapChain;

    // target view and texture
    ID3D11RenderTargetView* renderTargetView;

    ID3D11Texture2D* renderTexture = nullptr;
    ID3D11RenderTargetView* rtv = nullptr;
    ID3D11ShaderResourceView* srv = nullptr;

    // shadow mapping
    ID3D11Texture2D* shadowMap;
    ID3D11DepthStencilView* shadowMapDSV;

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

    // rasterizer
    ID3D11RasterizerState* wireframeRasterizerState;
    D3D11_RASTERIZER_DESC rasterizerDesc;

    // sampler
    ID3D11SamplerState* sampler;
    
    // constant buffer
    ID3D11Buffer* constantBuffer;
    D3D11_BUFFER_DESC constantBufferDesc;

    cbPerObject cbData; // the actual constant buffer object

    // lighting buffer
    ID3D11Buffer* lightBuffer;
    ID3D11Buffer* pointLightBuffer;
    D3D11_MAPPED_SUBRESOURCE mappedResource;

    // blend state
    ID3D11BlendState* blendState;

    // depth stencil
    ID3D11DepthStencilView* depthStencilView;
    ID3D11Texture2D* depthStencilBuffer;
    ID3D11DepthStencilState* depthStencilState;
    ID3D11ShaderResourceView* shadowMapSRV;

    // buffers
    ConstantBuffer<ColorBuffer>* colorBuffer;

    // we hold a vector of constant buffers that stay unitialized 

    // background color
    float backgroundColor[4];

    // engine
    Engine::GlowEngine* engine;
    // camera
    Visual::Camera* camera;
    // window
    Graphics::Window* window;

    // point lights - the renderer can have a variable amount of point lights
    int lights;
    bool fullscreen = false;

  };

} // graphics