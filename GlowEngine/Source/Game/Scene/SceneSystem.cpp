/*
/
// filename: SceneSystem.cpp
// author: Callen Betts
// brief: implements SceneSystem.h
/
*/

#include "stdafx.h"
#include "SceneSystem.h"
#include "ForestScene/ForestScene.h"

Scene::SceneSystem::SceneSystem(std::string systemName) 
  : System(systemName)
{
  scenes.push_back(new ForestScene());
  currentScene = scenes.at(0);
}

// update all entities within the scene
void Scene::SceneSystem::update()
{
  currentScene->update();
}

// render all the entities within a scene
void Scene::SceneSystem::render()
{
  currentScene->render();
}

// set the current scene
void Scene::SceneSystem::setCurrentScene(Scene* scene)
{
  // check if the scene was valid first
  if (!scene)
  {
    Logger::error("Scene " + scene->getName() + "was invalid");
    return;
  }
  // exit the scene
  currentScene->exit();
  // set the current scene
  currentScene = scene;
}

// exit the current scene
void Scene::SceneSystem::exitCurrentScene()
{

}
