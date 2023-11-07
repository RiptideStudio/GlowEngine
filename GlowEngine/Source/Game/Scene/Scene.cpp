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
  entityList = new Entities::EntityList();
  name = "Scene";
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
}

// create an entity within a scene that will directly add it to the list
// easy way to create new entities
Entities::Entity* Scene::Scene::createEntity(
  Vector3D pos, 
  Vector3D scale, 
  Vector3D rotation, 
  std::string modelName, 
  std::string textureName)
{
  Entities::Entity* entity = new Entities::Entity();
  entity->addComponent(new Components::Transform(pos, scale, rotation));
  entity->addComponent(new Components::Sprite3D(modelName,textureName));
  entityList->add(entity);
  return entity;
}


