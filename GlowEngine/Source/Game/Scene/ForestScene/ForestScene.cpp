/*
/
// filename: ForestScene.cpp
// author: Callen Betts
// brief: implements ForestScene.h
/
*/

#include "stdafx.h"
#include "ForestScene.h"

// initialize the forest scene
void Scene::ForestScene::init()
{
  createEntity({ -10,-10,-5 }, { 100,100,100 }, { 0 }, "Plane", "Stone");
  createEntity({ -10,15,-5 }, { 5,25,5 }, { 0 }, "Cube", "Cobblestone");
  createEntity({ 10,0,-5 }, { 40,15,2 }, { 0 }, "Cube", "Cobblestone");

  for (int i = 0; i < 15; ++i)
  {
    for (int j = 0; j < 15; ++j)
    {
      float randomX = randomRange(-100, 100);
      float randomZ = randomRange(-100, 100);
      float randomScale = randomRange(.5f, 2.f);

      float y = -10;
      Vector3D randomPos = { randomX,y,randomZ };
      Vector3D scale = { randomScale ,randomScale ,randomScale };
      Entities::Entity* e = createEntity(randomPos, scale, { randomRange(-30.f,30.f),0,randomRange(-30.f,30.f) }, "Crystal", "Cobblestone2");
       randomX = randomRange(-100, 100);
       randomZ = randomRange(-100, 100);

       y = -10;
        randomScale = randomRange(.4f, 1.f);

       randomPos = { randomX,y,randomZ };
       scale = { randomScale ,randomScale ,randomScale };
      Entities::Entity* e2 = createEntity(randomPos, scale, { randomRange(-30.f,30.f),0,randomRange(-30.f,30.f)}, "IcoSphere", "Cobblestone2");
      Components::Sprite3D* sprite = e->getComponent<Components::Sprite3D>(Components::ComponentType::sprite3D);
    }
  }
}

void Scene::ForestScene::update()
{
  updateEntities();
  // hotkeys for stress testing
  if (input->keyDown('G'))
  {
    float randomX = randomRange(-100, 100);
    float randomY = randomRange(0, 100);
    float randomZ = randomRange(-100, 100);
    float randomScale = randomRange(1, 5);
    float randomRotx = randomRange(0, 360);
    float randomRoty = randomRange(0, 360);
    float randomRotz = randomRange(0, 360);
    // createEntity({ randomX,randomY,randomZ }, { randomScale ,randomScale ,randomScale }, { randomRotx,randomRoty,randomRotz }, "Monkey", "StoneBrick");
    createEntity({ randomX,randomY,randomZ }, { randomScale ,randomScale ,randomScale }, { randomRotx,randomRoty,randomRotz }, "CircleCube", "Schilling");
    std::cout << "Entities: " << entityList->getSize() << std::endl;
  }
  if (input->keyDown('H'))
  {
    for (float i = 0; i < 50; i += 2)
    {
      for (float j = 0; j < 50; j += 2)
      {
        Entities::Entity* entity = new Entities::Entity();
        Components::Sprite3D* sprite = new Components::Sprite3D("Tree", "Cobblestone");

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

void Scene::ForestScene::exit()
{
}
