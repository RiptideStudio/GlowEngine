/*
/
// filename: Inspector.h
// author: Callen Betts
// brief: defines Inspector class
/
*/

#pragma once
#include "Engine/Graphics/UI/Editor/Widget.h"

namespace Entities
{
  class Entity;
}

namespace Editor
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