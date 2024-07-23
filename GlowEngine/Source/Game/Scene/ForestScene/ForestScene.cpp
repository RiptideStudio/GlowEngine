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

static Vector3D lightPosition = { 0,15,0 };
static DirectX::XMFLOAT4 lightColor = { 1,1,1,1 };
static float lightSize = 1;

// initialize the forest scene
void Scene::ForestScene::init()
{
  // player entity
  Entities::Entity* player = instanceCreateExt("Player", {20,5,0}, {1,2,1}, 0);
  player->addComponent(new Game::PlayerBehavior());
  player->addComponent(new Components::Physics());
  player->addComponent(new Components::BoxCollider({ 2,6,2 },false,false));

  Entities::Entity* plane = new Entities::Entity("Plane");
  plane->addComponent(new Components::Transform());
  plane->addComponent(new Components::Sprite3D());
  getComponentOfType(Sprite3D, plane)->setModel("Plane");
  getComponentOfType(Sprite3D, plane)->setTextures("Leaves");
  getComponentOfType(Sprite3D, plane)->setTextureRepeat(true);
  getComponentOfType(Transform, plane)->setScale({ 1000,1000,1000 });
  getComponentOfType(Transform, plane)->setPosition({ 0,-10,0 });
  add(plane);

  Entities::Actor* leaves = createEntity({ 0,-10,0 }, { 1000,0,1000 }, { 0 }, "", "");
  leaves->setAnchored(true);
  leaves->setTextureRepeat(true);
  leaves->setAsPointLight(true);
  leaves->updatePointLight(lightPosition, 75, lightColor);
  leaves->setName("Plane Leaves");

  Entities::Entity* m = instanceCreateExt("Monkey", { 5,0,-5 }, { 5,5,5 }, { 45,0,0 });
  m->addComponent(new Components::Physics());
  m->addComponent(new Components::BoxCollider());
  Entities::Actor* d = createEntity({ -10,5.56f,-5 }, { 3,3,3 }, { 0 }, "Chest", "Chest");

  for (int i = 0; i < 25; ++i)
  {
    for (int j = 0; j < 25; ++j)
    {
      float randomX = randomRange(-250, 250);
      float randomZ = randomRange(-250, 250);
      float randomScale = randomRange(1.5f, 2.f);

      float y = -10;
      Vector3D randomPos = { randomX,-4,randomZ };
      randomX = randomRange(-250, 250);
      randomZ = randomRange(-250, 250);
      Vector3D randomPos2 = { randomX,y,randomZ };
      randomX = randomRange(-250, 250);
      randomZ = randomRange(-250, 250);
      Vector3D randomPos3 = { randomX,0,randomZ };
      randomScale = randomRange(1.5f, 2.5f);

      Vector3D scale = { randomScale ,randomScale ,randomScale };
      Entities::Entity* r = instanceCreateExt("Tree", randomPos, scale, Vector3D(0, randomRange(0, 360), 0));
      r->addComponent(new Components::BoxCollider({2,8,2}, true, false));

      if (j % 5 == 0)
      {
        Entities::Entity* t = instanceCreateExt("Rock", randomPos2, scale, Vector3D(0, randomRange(0, 360), 0));
      }
    }
  }
}

void Scene::ForestScene::update()
{

}

void Scene::ForestScene::exit()
{

}
