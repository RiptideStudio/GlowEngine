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
Scene::Scene::Scene()
{
  engine = EngineInstance::getEngine();
  input = engine->getInputSystem();
  factory = engine->getEntityFactory();
  addEntityList();
  name = "Scene";
}

// exit a given scene
void Scene::Scene::exit()
{
  for (auto& wrapper : entityLists)
  {
    wrapper->list->clear();
  }
}

// render a scene
void Scene::Scene::render()
{
  renderEntities();
}

// update a scene's given entities
void Scene::Scene::updateEntities()
{
  for (auto& wrapper : entityLists)
  {
    wrapper->list->update();
  }
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
Entities::Actor* Scene::Scene::instanceCreate(std::string name, Vector3D position)
{
  Entities::Actor* actor = reinterpret_cast<Entities::Actor*>(factory->createEntity(name, position));
  entityList->add(actor);
  return actor;
}

Entities::Actor* Scene::Scene::instanceCreateExt(std::string name, Vector3D position, Vector3D scale, Vector3D rotation)
{
  Entities::Actor* actor = reinterpret_cast<Entities::Actor*>(factory->createEntity(name, position));
  actor->setScale(scale);
  actor->setRotation(rotation);
  add(actor);
  return actor;
}

Entities::Actor* Scene::Scene::instanceCreateGeneral(std::string name, std::string model, std::string texture, Vector3D position, Vector3D scale, Vector3D rotation)
{
  Entities::Actor* actor = reinterpret_cast<Entities::Actor*>(factory->createEntity(name, position));
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
}

void Scene::Scene::clear()
{
  for (auto& list : entityLists)
  {
    list->list->clear();
  }
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

