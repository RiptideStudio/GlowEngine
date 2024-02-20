/*
/
// filename: Physics.cpp
// author: Callen Betts
// brief: implements Physics.h
/
*/

#include "stdafx.h"
#include "Physics.h"
#include "Engine/GlowEngine.h"

Components::Physics::Physics()
{
  acceleration = { 0,gravity,0 };
  velocity = { 0,0,0 };
  targetVelocity = { 0,0,0 };
}

void Components::Physics::setVelocity(Vector3D vel)
{
  velocity = vel;
}

void Components::Physics::setTargetVelocity(Vector3D vel)
{
  targetVelocity = vel;
}

void Components::Physics::setAcceleration(Vector3D acc)
{
  acceleration = acc;
}

// update velocity and transform
void Components::Physics::update()
{
  float dt = EngineInstance::getEngine()->getDeltaTime();
  Components::Transform* transform = getComponentOfType(Transform, parent);

  if (transform)
  {
    // do a ground check (test)
    Vector3D position = transform->getPosition();

    if (position.y <= -8)
    {
      position.y = -8;
      acceleration = 0;
      return;
    }

    // set the velocity
    velocity += acceleration * dt;
    velocity.y = std::clamp(velocity.y, -maxVelocity, maxVelocity);

    Vector3D finalPosition = position + (velocity * dt);

    transform->setPosition(finalPosition);
  }
}

void Components::Physics::render()
{
}

