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
  Components::Sprite3D* sprite = getComponentOfType(Sprite3D, this);
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

// flag an entity for destroy
void Entities::Entity::destroy()
{
  destroyed = true;
}

// get a component
Components::Component* Entities::Entity::getComponent(Components::Component::ComponentType type)
{
  for (Components::Component* component : components)
  {
    if (component->getType() == type)
    {
      return component;
    }
  }
  return nullptr;
}
