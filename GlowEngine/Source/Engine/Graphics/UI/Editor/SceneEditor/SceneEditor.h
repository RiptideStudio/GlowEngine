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
  class EntityList;
  class EntityListWrapper;
  class EntityWrapper;
}

namespace Visual
{
  class Camera;
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

    // draws all the lists
    void DrawSceneHierarchy();
    // draws all of the entities in a list
    void DrawHierarchy(Entities::EntityList* entities);
    // draws a single entity
    void DrawEntity(Entities::Entity&);
    // find the parent of an entity list
    Entities::EntityList* FindParent(Entities::EntityList* root, Entities::EntityList* child);
    Entities::EntityList* FindEntityList(Entities::EntityList* root, Entities::Entity* child);

    // avoid duplicate logic
    void DragContainer(Entities::EntityListWrapper*, int);

  private:

    // quick pointers
    Scene::SceneSystem* sceneSystem;
    Scene::Scene* currentScene;
    Visual::Camera* camera;

    Entities::EntityList* selectedContainer;
    Entities::Entity* selectedEntity;

  };
}

