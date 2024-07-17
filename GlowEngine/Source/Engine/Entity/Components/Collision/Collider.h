/*
/
// filename: Collider.h
// author: Callen Betts
// brief: defines the Collider class, holds vertices and Collider data
/
*/

#pragma once
#include "Engine/Entity/Components/Component.h"
#include <set>

namespace Components
{

  class Collider : public Component
  {

  public:

    // the logic to check if we are colliding
    virtual bool isColliding(const Components::Collider* other) = 0;
    // while we are colliding
    virtual void onCollide(const Components::Collider* other) {};
    // when we touch for the first time
    virtual void onFirstCollide(const Components::Collider* other) {};
    // when we leave the collision
    virtual void onLeaveCollide(const Components::Collider* other) {};

    // render calls respective draw function
    void render();
    virtual void renderDebug() {};

    bool colliding = false; // if we are currently colliding
    bool collided = false;
    bool autoSize = true;
    bool isDirty = true; // recalculate mesh bounding box

    // objects we are currently colliding with
    std::set<const Components::Collider*> collidingObjects;
    std::vector<Vertex> vertices;

  protected:

    // debug drawing 
    bool debugDraw = true;
    std::vector<unsigned> indices;

  };

}