/*
/
// filename: Particle.h
// author: Callen Betts
// brief: defines Particle class
//
// description: Particles have the bare minimum properties to render on the screen. They are separated from entities
//  to maximize performance.
//
/
*/
#pragma once
#include "Engine/Entity/Entity.h"

namespace Particles
{

  class Particle : public Entities::Entity
  {

  public:

    Particle();

    // update the particle simulation
    void simulate();

  private:

    int lifeTime; // how long (in ticks) our particle lasts
    
  };

}