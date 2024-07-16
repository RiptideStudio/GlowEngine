/*
/
// filename: ForestScene.cpp
// author: Callen Betts
// brief: implements ForestScene.h
/
*/

#include "stdafx.h"
#include "ForestScene.h"
#include "Game/Behaviors/PlayerBehavior.h"

static Vector3D lightPosition = { -20,-10,-5 };
static DirectX::XMFLOAT4 lightColor = { 1.25,1,1,1 };
static float lightSize = 1;

// initialize the forest scene
void Scene::ForestScene::init()
{
  // player entity
  Entities::Actor* player = instanceCreateExt("Player", {0,5,0}, {1,2,1}, 0);
  player->addComponent(new Components::BoxCollider());
  player->addComponent(new Components::Physics());
  player->addComponent(new Game::PlayerBehavior());

  Entities::Actor* leaves = createEntity({ 0,-10,0 }, { 1000,0,1000 }, { 0 }, "", "");
  Entities::Entity* plane = new Entities::Entity();
  getComponentOfType(Sprite3D, plane)->setModel("Plane");
  getComponentOfType(Sprite3D, plane)->setTextures("Leaves");
  getComponentOfType(Sprite3D, plane)->setTextureRepeat(true);
  getComponentOfType(Transform, plane)->setScale({ 1000,1000,1000 });
  getComponentOfType(Transform, plane)->setPosition({0,-10,0});
  entityList->add(plane);
  leaves->setAnchored(true);
  leaves->setTextureRepeat(true);
  leaves->setAsPointLight(true);
  leaves->updatePointLight(leaves->getPosition(), 50, lightColor);
  instanceCreateExt("Monkey", { 10,10,-15 }, { 5,5,5 }, { 45,0,0 });
  instanceCreateExt("Soup", { 10,-4,-15 }, { 3,3,3 });
  //Entities::Actor* d = createEntity({ -10,17.56f,-5 }, { 1,1,1 }, { 0 }, "Door", "Cobblestone");
  //Entities::Actor* shroom = createEntity({ -30,-10,-15 }, { 1, 1, 1 }, { 0 }, "Mushroom", "Mushroom");

  for (int i = 0; i < 15; ++i)
  {
    for (int j = 0; j < 15; ++j)
    {
      float randomX = randomRange(-100, 100);
      float randomZ = randomRange(-100, 100);
      float randomScale = randomRange(1.5f, 2.f);

      float y = -10;
      Vector3D randomPos = { randomX,y,randomZ };
      randomX = randomRange(-100, 100);
      randomZ = randomRange(-100, 100);
      Vector3D randomPos2 = { randomX,y,randomZ };
      Vector3D scale = { randomScale ,randomScale ,randomScale };
      instanceCreateExt("Rock", randomPos2, scale, Vector3D(0, randomRange(0, 360), 0));
      if (j % 5 == 0)
      {
        instanceCreateExt("Tree", randomPos, scale, Vector3D(0, randomRange(0, 360), 0));
      }
    }
  }
}

void Scene::ForestScene::update()
{
  // hotkeys for stress testing
  if (input->keyDown('G'))
  {
    float randomX = randomRange(-100.f, 100.f);
    float randomY = randomRange(0.f, 100.f);
    float randomZ = randomRange(-100.f, 100.f);
    float randomScale = randomRange(1.f, 5.f);
    float randomRotx = randomRange(0.f, 360.f);
    float randomRoty = randomRange(0.f, 360.f);
    float randomRotz = randomRange(0.f, 360.f);
    createEntity({ randomX,randomY,randomZ }, { randomScale ,randomScale ,randomScale }, { randomRotx,randomRoty,randomRotz }, "Tree", "Leaves");
    std::cout << "Entities: " << entityList->getSize() << std::endl;
  }
}

void Scene::ForestScene::exit()
{

}
