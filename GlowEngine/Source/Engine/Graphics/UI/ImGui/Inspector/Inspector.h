/*
/
// filename: Inspector.h
// author: Callen Betts
// brief: defines Inspector class
/
*/

#pragma once
#include "Engine/Graphics/UI/ImGui/Widget.h"

namespace UI
{
  class Inspector : public Widget
  {

  public:

    Inspector(std::string title, std::string desc = "", ImGuiWindowFlags flags = 0);

    void render();

  };
}