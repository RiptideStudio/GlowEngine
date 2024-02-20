/*
/
// filename: Physics.h
// author: Callen Betts
// brief: defines the Physics class, holds vertices and Physics data
/
*/

#pragma once
#include "Engine/Entity/Components/Component.h"

namespace Components
{

  class Physics : public Component
  {

  public:

    Physics();

    void setVelocity(Vector3D vel);
    void setTargetVelocity(Vector3D vel);
    void setAcceleration(Vector3D acc);

    void update();

    void render();

  private:

    Vector3D velocity;
    Vector3D targetVelocity;
    Vector3D acceleration;

    // this is the gravity constant
    const float gravity = -15.f;
    const float maxVelocity = 50.f;

  };

}