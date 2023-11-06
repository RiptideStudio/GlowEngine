/*
/
// filename: Scene.cpp
// author: Callen Betts
// brief: implements Scene.h
/
*/

#include "stdafx.h"
#include "Scene.h"
#include "Engine/GlowEngine.h"

// base scene constructor
Scene::Scene::Scene()
{
  engine = EngineInstance::getEngine();
  input = engine->getInputSystem();
  entityList = new Entities::EntityList();
  name = "Scene";
  init();
}

// test for entities
void Scene::Scene::init()
{
  createEntity({ 0,0,4 }, { 3,3,-3 }, { 0 }, "Monkey");
  createEntity({ -10,-10,-5 }, { 100,1,100 }, { 0 }, "Plane", "StoneBrick");
}

void Scene::Scene::update()
{
  // hotkeys for stress testing
  if (input->keyDown('G'))
  {
    float randomX = randomRange(-100, 100);
    float randomY = randomRange(0, 100);
    float randomZ = randomRange(-100, 100);
    float randomScale = randomRange(1,5);
    float randomRotx = randomRange(0,360);
    float randomRoty = randomRange(0,360);
    float randomRotz = randomRange(0,360);
    // createEntity({ randomX,randomY,randomZ }, { randomScale ,randomScale ,randomScale }, { randomRotx,randomRoty,randomRotz }, "Monkey", "StoneBrick");
    createEntity({ randomX,randomY,randomZ }, { randomScale ,randomScale ,randomScale }, { randomRotx,randomRoty,randomRotz }, "Torus", "Cobblestone");
    std::cout << "Entities: " << entityList->getSize() << std::endl;
  }
  if (input->keyDown('H'))
  {
    for (float i = 0; i < 50; i += 2)
    {
      for (float j = 0; j < 50; j += 2)
      {
        Entities::Entity* entity = new Entities::Entity();
        Components::Sprite3D* sprite = new Components::Sprite3D("Tree","Cobblestone");

        if ((int)i % 5 == 0)
        {
          sprite->setModel("Cube");
        }
        else if ((int)j % 5 == 0)
        {
          sprite->setModel("IcoSphere");
        }
        else
        {
          sprite->setModel("Cylinder");
        }

        entity->addComponent(sprite);
        entity->addComponent(new Components::Transform({ 5 - i,j - 5,10 + sinf(i) * j }, { 1,1,1 }, { i,j,1 }));
        entityList->add(entity);
      }
    }
  }
  if (input->keyDown('R'))
  {
    entityList->clear();
  }
}

void Scene::Scene::exit()
{

}

void Scene::Scene::updateEntities()
{
  entityList->update();
}

void Scene::Scene::renderEntities()
{
  entityList->render();
}

Entities::Entity* Scene::Scene::createEntity(Vector3D pos, Vector3D scale, Vector3D rotation, std::string modelName, std::string textureName)
{
  Entities::Entity* entity = new Entities::Entity();
  entity->addComponent(new Components::Transform(pos, scale, rotation));
  entity->addComponent(new Components::Sprite3D(modelName,textureName));
  entityList->add(entity);
  return entity;
}


