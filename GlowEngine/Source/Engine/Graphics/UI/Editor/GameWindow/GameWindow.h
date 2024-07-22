/*
/
// filename: GameWindow.h
// author: Callen Betts
// brief: defines GameWindow class
/
*/

#pragma once
#include "Engine/Graphics/UI/Editor/Widget.h"

namespace Editor
{
  class GameWindow : public Widget
  {

  public:

    GameWindow(std::string title, std::string desc = "", ImGuiWindowFlags flags = 0) : Widget(title, desc, flags)
    {
      flags = ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;
    }
    
    void update();
    void calculateGameWindowSize();

    Entities::Entity* RayPickEntity(Vector3D vector, DirectX::XMMATRIX view, DirectX::XMMATRIX perspective);

  private:

    // padding for our game window so it clamps correctly
    float xPadding = 16;
    float yPadding = 16;

    // game window properties
    ImVec2 availableSize;
  };
}