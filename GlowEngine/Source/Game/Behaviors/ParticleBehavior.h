/*
/
// filename: ParticleBehavior.h
// author: Callen Betts
// brief: defines ParticleBehavior class
/
*/

#pragma once
#include "Game/Behaviors/Behavior.h"

namespace Game
{

  class ParticleBehavior : public Components::Component
  {

  public:

    ParticleBehavior();

    void update();

  };

}

