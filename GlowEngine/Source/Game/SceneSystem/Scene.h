/*
/
// filename: Scene.h
// author: Callen Betts
// brief: defines base scene class
/
*/

#pragma once
#include "Engine/Entity/EntityList/EntityList.h"
#include "Engine/Entity/Entity.h"

namespace SceneSystem
{

  class Scene
  {

  public:

    Scene();

    virtual void init();
    virtual void update();
    virtual void exit();

  private:

    std::string name;
    Entities::EntityList* entityList;

  };

}