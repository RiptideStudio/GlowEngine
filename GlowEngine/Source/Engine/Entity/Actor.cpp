/*
/
// filename: Actor.cpp
// author: Callen Betts
// brief: implements Actor.h
//
// description: The actor class is an abstracted version of entity. It provides us an interface to easily
//  interact with our entities and their components without having to do any sort of gross "get" chains
//
/
*/

#include "stdafx.h"
#include "Actor.h"
#include "Engine/EngineInstance.h"
#include "Engine/GlowEngine.h"
#include "Game/Scene/Scene.h"
#include "Game/Scene/SceneSystem.h"
#include "Engine/Graphics/Renderer.h"

// construct actors with premade components
Entities::Actor::Actor()
  : isLight(false),
  light(nullptr)
{
  engine = EngineInstance::getEngine();
  renderer = engine->getRenderer();
  transform = new Components::Transform();
  addComponent(transform);
  physics = new Components::Physics();
  addComponent(physics);
  sprite = new Components::Sprite3D("Cube");
  addComponent(sprite);
  addComponent(new Components::BoxCollider());
  engine->getSceneSystem()->getCurrentScene()->getEntityList()->add(this); // adds actors to active list
}

// ** Physics ** //
void Entities::Actor::setAcceleraton(Vector3D acc)
{
  physics->setAcceleration(acc);
}

void Entities::Actor::setVelocity(Vector3D vel)
{
  physics->setVelocity(vel);
}

// ** Models ** //
// set the model
void Entities::Actor::setModel(std::string name)
{
  sprite->setModel(name);
}

// ** Textures ** //
// set the texture
void Entities::Actor::setTexture(std::string name)
{
  sprite->setTextures(name);
}

void Entities::Actor::setTextureRepeat(bool val)
{
  sprite->setTextureRepeat(val);
}

// ** Lighting ** //
// initialize the point light and set it as active
void Entities::Actor::setAsPointLight(bool val)
{
  // toggle the light on and off
  isLight = val;

  // create the light if it does not exist
  if (!light)
  {
    createPointLight();
  }
}

// update the size of the light
void Entities::Actor::setLightSize(float size)
{
  light->pointLight.size = size;
}

// create the point light
void Entities::Actor::createPointLight()
{
  light = new PointLight();
  updatePointLight(getPosition(), 2, { 2.5,1.5,1,1 });
  renderer->addPointLight(light);
}

// create a new pointlight data struct and add it to the renderer's list of lights
void Entities::Actor::updatePointLight(Vector3D pos, float size, DirectX::XMFLOAT4 color)
{
  light->pointLight.color = color;
  light->pointLight.position = { pos.x,pos.y,pos.z };
  light->pointLight.size = size;
  renderer->updatePointLight(light);
}


