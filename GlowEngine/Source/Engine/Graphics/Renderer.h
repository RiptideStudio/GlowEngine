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

  // define a constant buffer to be sent to the vertex shader
  struct cbPerObject
  {
    DirectX::XMMATRIX world;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
  };

  // define a point light buffer
  struct LightBuffer
  {
    DirectX::XMFLOAT3 lightDirection; // Ensure 16-byte aligned if you add more fields.
    float padding;
    DirectX::XMFLOAT3 lightColor;
    float padding2;
    DirectX::XMFLOAT3 cameraPosition; // Ensure 16-byte aligned if you add more fields.
    float padding3;
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

    // get the directX devices for draw calls
    ID3D11Device* getDevice();
    ID3D11DeviceContext* getDeviceContext();

  private:

    // window handle
    HWND windowHandle;

    // devices
    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;
    IDXGISwapChain* swapChain;
    ID3D11RenderTargetView* renderTargetView;

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
    D3D11_MAPPED_SUBRESOURCE mappedResource;

    // blend state
    ID3D11BlendState* blendState;

    // depth stencil
    ID3D11DepthStencilView* depthStencilView;
    ID3D11Texture2D* depthStencilBuffer;
    ID3D11DepthStencilState* depthStencilState;

    // background color
    float backgroundColor[4];

    // engine
    Engine::GlowEngine* engine;
    // camera
    Visual::Camera* camera;

  };

} // graphics