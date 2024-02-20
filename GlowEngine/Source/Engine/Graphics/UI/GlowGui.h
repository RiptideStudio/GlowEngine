/*
/
// filename: GlowGui.h
// author: Callen Betts
// brief: defines GlowGui class
/
*/

#pragma once

namespace Graphics
{

  class GlowGui
  {

  public:

    GlowGui(HWND windowHandle, ID3D11Device* device, ID3D11DeviceContext* context, Graphics::Renderer* renderers);

    void beginUpdate();
    void update();
    void endUpdate();

    void cleanUp();

  private:

    HWND windowHandle;
    ID3D11Device* device;
    ID3D11DeviceContext* context;
    Graphics::Renderer* renderer;

  };

}