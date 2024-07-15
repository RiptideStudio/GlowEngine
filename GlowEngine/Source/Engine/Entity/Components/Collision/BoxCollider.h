/*
/
// filename: BoxCollider.h
// author: Callen Betts
// brief: defines the BoxCollider class, holds vertices and BoxCollider data
/
*/

#pragma once
#include "Engine/Entity/Components/Component.h"
#include "Engine/Entity/Components/Collision/Collider.h"

namespace Components
{

  class BoxCollider : public Collider
  {

  public:

    BoxCollider(Vector3D newScale);

    virtual bool isColliding(const Components::Collider*) override;

    virtual void onFirstCollide();
    virtual void onCollide();
    virtual void onLeaveCollide();

    virtual void update();

  private:

    Vector3D scale;

  };

}