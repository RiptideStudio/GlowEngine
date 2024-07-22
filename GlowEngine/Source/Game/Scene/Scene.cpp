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

// base scene constructor
Scene::Scene::Scene()
{
  engine = EngineInstance::getEngine();
  input = engine->getInputSystem();
  factory = engine->getEntityFactory();
  globalList = new Entities::EntityList();
  entityList = new Entities::EntityList();
  addEntityList();
  name = "Scene";
}

// exit a given scene
void Scene::Scene::exit()
{
  clear();
}

// render a scene
void Scene::Scene::render()
{
  renderEntities();
}

// update a scene's given entities
void Scene::Scene::updateEntities()
{
  // update all of our entity lists
  for (auto& wrapper : entityLists)
  {
    wrapper->list->update();
  }

  // check for collisions in our global list
  globalList->updateColliders();
}

// render a scene's entities
void Scene::Scene::renderEntities()
{
  for (auto& wrapper : entityLists)
  {
    wrapper->list->render();
  }
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
  entityLists[0]->list->add(entity);
  entityList->add(entity);
}

void Scene::Scene::addToList(Entities::EntityList* list, Entities::Entity* ent)
{
  list->add(ent);
}

void Scene::Scene::clear()
{
  for (auto& wrapper : entityLists)
  {
    wrapper->list->clear();
  }
  globalList->clear();
  entityList->clear();
}

void Scene::Scene::ReorderLists(int src, int dst)
{
  auto list = entityLists[src];
  entityLists.erase(entityLists.begin() + src);
  entityLists.insert(entityLists.begin() + dst, list);
}

// creates a new entity wrapper given a name
void Scene::Scene::addEntityList(std::string name)
{
  Entities::EntityList* list = new Entities::EntityList();
  Entities::EntityListWrapper* wrapper = new Entities::EntityListWrapper(list);

  size_t size = entityLists.size();

  wrapper->name = name;

  if (size > 0)
    wrapper->name += std::to_string(size);

  entityLists.push_back(wrapper);
}

// pick an entity from a scene given an origin vector and a direction, finds the first one
Entities::Entity* Scene::Scene::RayPick(Vector3D origin, Vector3D dir)
{
  float closestDistance = FLT_MAX;
  Entities::Entity* closestEntity = nullptr;

  for (const auto& entity : entityList->getEntities())
  {
    Components::BoundingBox* box = getComponentOfType(BoundingBox,entity);

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

