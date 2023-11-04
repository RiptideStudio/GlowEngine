/*
/
// filename: Scene.cpp
// author: Callen Betts
// brief: implements Scene.h
/
*/

#include "stdafx.h"
#include "Scene.h"
#include "Engine/Entity/EntityList/EntityList.h"
#include "Engine/Entity/Entity.h"

// base scene constructor
SceneSystem::Scene::Scene()
{
  name = "Scene";
  entityList = new Entities::EntityList();
}

// test for entities
void SceneSystem::Scene::init()
{
  for (float i = 0; i < 50; i+=2)
  {
    for (float j = 0; j < 50; j+=2)
    {
      Entities::Entity* entity = new Entities::Entity();
      entity->addComponent(new Components::Model());
      entity->addComponent(new Components::Transform({ 5-i,j-5,10+sinf(i)*j}, {1,1,1}, {i,j,i * j}));
      entityList->add(entity);
    }
  }
}

void SceneSystem::Scene::update()
{
  entityList->update();
}

void SceneSystem::Scene::exit()
{
}
