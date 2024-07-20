/*
/
// filename: Inspector.h
// author: Callen Betts
// brief: defines Inspector class
/
*/

#pragma once
#include "Engine/Graphics/UI/ImGui/Widget.h"

namespace Entities
{
  class Entity;
}

namespace UI
{
  class Inspector : public Widget
  {

  public:

    Inspector(std::string title, std::string desc = "", ImGuiWindowFlags flags = 0);

    void update();

    // the entity we want to inspect
    void inspect(Entities::Entity* ent) { selectedEntity = ent; }

    Entities::Entity* selectedEntity;

  };
}