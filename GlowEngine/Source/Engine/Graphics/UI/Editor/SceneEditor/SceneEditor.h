/*
/
// filename: SceneEditor.h
// author: Callen Betts
// brief: Implements a scene editor where we can add and remove entity lists
/
*/

#pragma once
#include "Engine/Graphics/UI/Editor/Widget.h"


namespace Entities
{
  class EntityList; // forward declare
  class EntityListWrapper;
}

namespace Editor
{
  class Inspector; // forward declare

  class SceneEditor : public Widget
  {

  public:

    SceneEditor(std::string title, std::string desc = "", ImGuiWindowFlags flags = 0);

    void init();
    void update();
    void interact();

    // avoid duplicate logic
    void DragContainer(Entities::EntityListWrapper*, int);

  private:

    // quick pointers
    Scene::SceneSystem* sceneSystem;
    Scene::Scene* currentScene;

    Entities::EntityListWrapper* selectedContainer;
    Entities::Entity* selectedEntity;

  };
}

