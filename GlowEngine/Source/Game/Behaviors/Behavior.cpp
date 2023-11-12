/*
/
// filename: Behavior.cpp
// author: Callen Betts
// brief: implements Behavior.h
/
*/

#include "stdafx.h"
#include "Behavior.h"

// create a behavior
Game::Behavior::Behavior()
  : Component()
{
  type = Components::Component::ComponentType::Behavior;
}

