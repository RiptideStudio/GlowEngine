/*
/
// filename: Component.cpp
// author: Callen Betts
// brief: implements Component.h
/
*/

#include "stdafx.h"
#include "Component.h"
#include "Engine/Entity/Entity.h"

// base component constructor
Components::Component::Component()
  :
  parent(nullptr)
{
  type = ComponentType::None;
}

// get the type
const Components::Component::ComponentType Components::Component::getType()
{
  return type;
}

// get the parented entity
Entities::Entity* Components::Component::getParent()
{
  return parent;
}

void Components::Component::setParent(Entities::Entity* newParent)
{
  parent = newParent;
}
