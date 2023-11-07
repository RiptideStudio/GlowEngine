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

// exit the current scene
void Scene::SceneSystem::exitCurrentScene()
{

}
