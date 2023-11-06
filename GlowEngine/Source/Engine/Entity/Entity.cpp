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
  }
}

// render an entity's components - typically this is a Sprite3D
void Entities::Entity::render()
{
  // get the sprite3D to render
  Components::Sprite3D* sprite = getComponent<Components::Sprite3D>(Components::ComponentType::sprite3D);
  if (sprite)
  {
    sprite->render();
  }
}

// attach a new component to an entity
void Entities::Entity::addComponent(Components::Component* component)
{
  component->setParent(this);
  this->components.push_back(component);
}
