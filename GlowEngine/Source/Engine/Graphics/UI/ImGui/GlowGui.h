/*
/
// filename: GlowGui.h
// author: Callen Betts
// brief: defines GlowGui class
/
*/

#pragma once

namespace UI
{
  class Widget;
}

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
    void calculateGameWindowSize();

  private:

    HWND windowHandle;
    ID3D11Device* device;
    ID3D11DeviceContext* context;
    Graphics::Renderer* renderer;

    // game window properties
    ImVec2 availableSize;

    // padding for our game window so it clamps correctly
    float xPadding = 16;
    float yPadding = 16;

    std::vector<UI::Widget*> widgets;
    ImGuiWindowFlags gameWindowFlags = ImGuiWindowFlags_NoBackground |
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
      ImGuiWindowFlags_NoScrollWithMouse;

  };

}