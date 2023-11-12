/*
/
// filename: ParticleBehavior.cpp
// author: Callen Betts
// brief: implements ParticleBehavior.h
/
*/

#include "stdafx.h"
#include "ParticleBehavior.h"
#include "Engine/Graphics/Particles/Particle.h"

Game::ParticleBehavior::ParticleBehavior()
{
}

// particle behaviors
void Game::ParticleBehavior::update()
{
  // gross cast to particle parent, this way we can access particle properties directly
  Particles::Particle* part = static_cast<Particles::Particle*>(parent);

  part->simulate();
}
