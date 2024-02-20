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

    // define collider types
    enum class ColliderType
    {
      Box,
      Sphere
    };

  };

}