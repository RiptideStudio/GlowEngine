/*
/
// filename: SceneSystem.h
// author: Callen Betts
// brief: defines the scene system class
/
*/

#pragma once
#include "Game/Scene/Scene.h"
#include "ForestScene/ForestScene.h"

namespace Scene
{

  class SceneSystem : public Systems::System
  {

  public:

    // call the constructor as well as the base system constructor
    SceneSystem(std::string systemName) : System(systemName)
    {
      currentScene = static_cast<Scene*>(new ForestScene());
      scenes.push_back(currentScene);
    }
    
    // update the entities in all scenes
    void update();
    // render the entities in all scenes
    void render();

    // exit a scene
    void exitCurrentScene();

    // retrieve the current scene
    Scene* getCurrentScene() { return currentScene; }

  private:

    // the scene that is currently active
    Scene* currentScene;

    // we have a vector of scenes that we iteratively update
    std::vector<Scene*> scenes;

  };

}