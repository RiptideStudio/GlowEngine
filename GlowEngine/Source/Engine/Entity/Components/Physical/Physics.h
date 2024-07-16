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

    void setAccelerationY(float val) { acceleration.x = val; }

    void setVelocityX(float val);
    void setVelocityY(float val);
    void setVelocityZ(float val);

    const Vector3D getVelocity() { return velocity; }
    const Vector3D getAcceleration() { return acceleration; }

    void update();

    void render();

    void setAnchored(bool val);
    void setGrounded(bool val) { grounded = val; }
    bool isGrounded() { return grounded; }

  private:

    Vector3D velocity;
    Vector3D targetVelocity;
    Vector3D acceleration;

    // anchored means we can't move no matter what
    bool anchored = false;
    bool grounded = false; // we are on the ground

    // this is the gravity constant
    const float gravity = -25.f;
    const float maxVelocity = 100.f;

  };

}