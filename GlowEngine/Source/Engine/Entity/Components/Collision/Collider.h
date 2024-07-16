/*
/
// filename: Collider.h
// author: Callen Betts
// brief: defines the Collider class, holds vertices and Collider data
/
*/

#pragma once
#include "Engine/Entity/Components/Component.h"

namespace Components
{

  class Collider : public Component
  {

  public:

    // the logic to check if we are colliding
    virtual bool isColliding(const Components::Collider* other) = 0;
    // while we are colliding
    virtual void onCollide() {};
    // when we touch for the first time
    virtual void onFirstCollide() {};
    // when we leave the collision
    virtual void onLeaveCollide() {};

    // define collider types
    enum class ColliderType
    {
      Box,
      Sphere
    };

    bool colliding = false; // if we are currently colliding
    bool collided = false;

  protected:

    bool autoSize = true;
    bool isDirty = true; // recalculate mesh bounding box

  };

}