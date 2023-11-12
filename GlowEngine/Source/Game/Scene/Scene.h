/*
/
// filename: Scene.h
// author: Callen Betts
// brief: defines base scene class
/
*/

#pragma once
#include "Engine/Entity/EntityList/EntityList.h"
#include "Engine/Graphics/Particles/Particle.h"
#include "Engine/Graphics/Particles/ParticleEmitter.h"
#include "Engine/Entity/Entity.h"
#include "Engine/GlowEngine.h"

namespace Scene
{

  class Scene
  {

  public:

    Scene();

    virtual void init() {};
    virtual void update() {};

    // exit a scene
    void exit();

    virtual void render();

    void updateEntities();
    void renderEntities();

    // get a scene's entity container
    Entities::EntityList* getEntityList() { return entityList; }

    std::string getName() { return name; }

    // create an entity directly to this scene with already made components
    Entities::Actor* createEntity(Vector3D pos, Vector3D scale, Vector3D rotation, std::string modelName, std::string textureName = "");

  protected:

    // system pointers
    Engine::GlowEngine* engine;
    Input::InputSystem* input;

    std::string name;

    // entity list
    Entities::EntityList* entityList;
    // particle list
    Entities::EntityList* particleList;

  };

}