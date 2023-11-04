/*
/
// filename: Entity.cpp
// author: Callen Betts
// brief: implements Entity.h
/
*/

#include "stdafx.h"
#include "Entity.h"

// base entity constructor
Entities::Entity::Entity()
  :
  id(0),
  name("Entity"),
  destroyed(false)
{
  
}

// virtual destructor for entities
Entities::Entity::~Entity()
{
  // no-op
}

// update all the components of an entity
void Entities::Entity::update()
{
  for (auto component : components)
  {
    component->update();
    component->render();
  }
}

// attach a new component to an entity
void Entities::Entity::addComponent(Components::Component* component)
{
  component->setParent(this);
  this->components.push_back(component);
}
