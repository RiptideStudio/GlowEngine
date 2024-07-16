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
  entityList = new Entities::EntityList();
  particleList = new Entities::EntityList();
  name = "Scene";
}

// exit a given scene
void Scene::Scene::exit()
{
  entityList->clear();
}

// render a scene
void Scene::Scene::render()
{
  renderEntities();
}

// update a scene's given entities
void Scene::Scene::updateEntities()
{

  entityList->update();
}

// render a scene's entities
void Scene::Scene::renderEntities()
{
  entityList->render();
  particleList->render();
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
  entityList->add(actor);
  return actor;
}

Entities::Actor* Scene::Scene::instanceCreateGeneral(std::string name, std::string model, std::string texture, Vector3D position, Vector3D scale, Vector3D rotation)
{
  Entities::Actor* actor = reinterpret_cast<Entities::Actor*>(factory->createEntity(name, position));
  actor->setScale(scale);
  actor->setRotation(rotation);
  actor->setModel(model);
  actor->setTexture(texture);
  entityList->add(actor);
  return actor;
}

