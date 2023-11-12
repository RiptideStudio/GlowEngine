/*
/
// filename: Particle.cpp
// author: Callen Betts
// brief: implements Particle.h
/
*/

#include "stdafx.h"
#include "Particle.h"
#include "Engine/GlowEngine.h"
#include "Engine/EngineInstance.h"
#include "Game/Scene/SceneSystem.h"
#include "Game/Behaviors/ParticleBehavior.h"

static Vector3D rot;

// give our particles a sprite and a transform - this is all they need 
Particles::Particle::Particle()
  : Entity(),
  lifeTime(240)
{
  addComponent(new Components::Transform({ 0,0,0 }, { .1,.1,.1 }, { 0,0,0 }));
  addComponent(new Components::Sprite3D("Cube","Sun"));
  Components::Sprite3D* s = getComponentOfType(Sprite3D, this);
  s->setColor({ 1,3,1,1 });
  addComponent(new Game::ParticleBehavior());
}

// update a particle's simulation based on its properties
void Particles::Particle::simulate()
{
  // cast our parent to a particle type
  Components::Transform* transform = getComponentOfType(Transform, this);

  float dt = EngineInstance::getEngine()->getDeltaTime();
  Vector3D newPos = transform->getPosition();
  newPos.y += 10 * dt;
  transform->setPosition(newPos);
  rot.x += .5*dt;
  rot.y += .3*dt;
  rot.z += .4*dt;
  transform->setRotation(rot);
  lifeTime -= 1;

  if (lifeTime <= 0)
  {
    destroy();
  }
}

