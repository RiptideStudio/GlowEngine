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

    BoxCollider();

  private:

    Vector3D scale;

  };

}