/*
/
// filename: Scene.cpp
// author: Callen Betts
// brief: implements Scene.h
/
*/

#include "stdafx.h"
#include "Scene.h"
#include "Engine/Graphics/Renderer.h"
#include "Engine/Graphics/Camera/Camera.h"

// base scene constructor
Scene::Scene::Scene()
{
  engine = EngineInstance::getEngine();
  input = engine->getInputSystem();
  factory = engine->getEntityFactory();
  globalList = new Entities::EntityList();
  rootList = new Entities::EntityList();
  rootList->add(engine->getCamera());
  rootList->SetName("Root");
  name = "Scene";
}

// exit a given scene; this doesn't do anything right now, because we have no data we want to unload
// this may be useful in the future, but for now it will just restart the scene
void Scene::Scene::exit()
{
  restart();
}

void Scene::Scene::restart()
{
  clear();
  init();
}

// render a scene
void Scene::Scene::render()
{
  renderEntities();
}

// update a scene's given entities
void Scene::Scene::updateEntities()
{
  // update all of our entity lists; entity lists recursively update their sublists
  rootList->update();

  // we have a separate list for handling collisions
  globalList->updateColliders();
}

// render a scene's entities
void Scene::Scene::renderEntities()
{
  rootList->render();
}

// create an entity within a scene that will directly add it to the list
// easy way to create new entities
Entities::Actor* Scene::Scene::createEntity(
  Vector3D pos, 
  Vector3D scale, 
  Vector3D rotation, 
  std::string modelName, 
  std::string textureName)
{
  Entities::Actor* actor = new Entities::Actor();
  actor->setPosition(pos);
  actor->setScale(scale);
  actor->setRotation(rotation);
  actor->setModel(modelName);
  actor->setTexture(textureName);
  add(actor);
  return actor;
}

// boilerplate for adding an entity directly to the scene
// lets you fully customize your entity with different complexity levels
Entities::Entity* Scene::Scene::instanceCreate(std::string name, Vector3D position)
{
  Entities::Entity* actor = factory->createEntity(name, position);
  actor->transform->setPosition(position);
  add(actor);
  return actor;
}

Entities::Entity* Scene::Scene::instanceCreateExt(std::string name, Vector3D position, Vector3D scale, Vector3D rotation)
{
  Entities::Entity* actor = factory->createEntity(name, position);
  actor->transform->setScale(scale);
  actor->transform->setRotation(rotation);
  actor->transform->setPosition(position);
  actor->sprite->setModel(name);
  add(actor);
  return actor;
}

Entities::Actor* Scene::Scene::instanceCreateGeneral(std::string name, std::string model, std::string texture, Vector3D position, Vector3D scale, Vector3D rotation)
{
  Entities::Actor* actor = factory->createActor(name, position);
  actor->setScale(scale);
  actor->setRotation(rotation);
  actor->setModel(model);
  actor->setTexture(texture);
  add(actor);
  return actor;
}

// add an entity to the root node list; we'll eventually be using the scene hierarchy to do most of this
void Scene::Scene::add(Entities::Entity* entity)
{
  rootList->add(entity);
  entity->SetId(rootList->getSize());
}

void Scene::Scene::clear()
{
  for (auto& wrapper : entityLists)
  {
    wrapper->list->clear();
  }
  globalList->clear();
  rootList->clear();
}

void Scene::Scene::ReorderLists(int src, int dst)
{
  auto list = entityLists[src];
  entityLists.erase(entityLists.begin() + src);
  entityLists.insert(entityLists.begin() + dst, list);
}

// pick an entity from a scene given an origin vector and a direction, finds the first one
Entities::Entity* Scene::Scene::RayPick(Vector3D origin, Vector3D dir)
{
  float closestDistance = FLT_MAX;
  Entities::Entity* closestEntity = nullptr;

  for (const auto& entity : rootList->getEntities())
  {
    if (!entity->isVisible())
      continue;

    Components::BoundingBox* box = getComponentOfType(BoundingBox,entity);
    if (!box)
      continue;

    if ((box->min == 0 && box->max == 0))
      continue;

    float distance;
    if (Vector3D::RayIntersectsBoundingBox(origin, dir, box, distance))
    {
      if (distance < closestDistance)
      {
        closestDistance = distance;
        closestEntity = entity;
      }
    }
  }

  return closestEntity;
}

