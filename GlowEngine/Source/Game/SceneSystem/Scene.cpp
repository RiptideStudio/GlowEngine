/*
/
// filename: Scene.cpp
// author: Callen Betts
// brief: implements Scene.h
/
*/

#include "stdafx.h"
#include "Scene.h"

// base scene constructor
SceneSystem::Scene::Scene()
{
  name = "Scene";
  entityList = new Entities::EntityList();
}

// test for entities
void SceneSystem::Scene::init()
{
  entityList->add(Entities::createEntity({ 0,0,5 }, "Monkey"));

  for (float i = 0; i < 25; i += 2)
  {
    for (float j = 0; j < 25; j += 2)
    {
      Entities::Entity* entity = new Entities::Entity();
      Components::Sprite3D* sprite = new Components::Sprite3D();

      if ((int)i % 5 == 0)
      {
        sprite->setModel("Monkey");
      }
      else if((int)j % 5 == 0)
      {
        sprite->setModel("Monkey");
      }
      else
      {
        sprite->setModel("Monkey");
      }
      sprite->setColor({ sinf(i), sinf(j), cosf(i), 1});

      entity->addComponent(sprite);
      entity->addComponent(new Components::Transform({ 5 - i,j - 5,10 + sinf(i) * j }, { 1,1,1 }, { i,j,1}));
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
