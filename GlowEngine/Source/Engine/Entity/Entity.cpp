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
  addComponent(transform = new Components::Transform());
  addComponent(sprite = new Components::Sprite3D("Cube"));
}

// copy constructor for entity
Entities::Entity::Entity(const Entity& other)
  :
  id(0),
  name("Entity"),
  destroyed(false),
  visible(other.visible)
{
  // copy components
  for (const auto& component : other.components)
  {
    Components::Component* comp = component->clone();
    addComponent(comp);
  }

  // update quick pointers
  transform = getComponentOfType(Transform, this);
  sprite = getComponentOfType(Sprite3D, this);
  physics = getComponentOfType(Physics, this);
}

// virtual destructor for entities
Entities::Entity::~Entity()
{
  for (auto component : components)
  {
    delete component;
  }
}

// when loading an entity from json, iterate over its components and call their load method
void Entities::Entity::load(const nlohmann::json& data)
{
  if (data.contains("name")) setName(data["name"]);
  if (data.contains("visible")) visible = data["visible"];

  for (const auto& component : components)
  {
    component->load(data);
  }
}

// update all the components of an entity
void Entities::Entity::update()
{
  for (auto& component : components)
  {
    component->update();
  }
}

// render an entity's components - typically this is a Sprite3D
void Entities::Entity::render()
{
  // get the sprite3D to render
  for (auto component : components)
  {
    component->render();
  }
}

// attach a new component to an entity
void Entities::Entity::addComponent(Components::Component* component)
{
  // add the components
  component->setParent(this);
  this->components.push_back(component);

  // sort them in order of priority, higher priority goes first
  std::sort(components.begin(), components.end(), [](Components::Component* a, Components::Component* b) {
    return a->getPriority() > b->getPriority();
  });
}

// flag an entity for destroy
void Entities::Entity::destroy()
{
  destroyed = true;
}

void Entities::Entity::setName(std::string newName)
{
  name = newName;
}

// has a component
bool Entities::Entity::hasComponent(Components::Component::ComponentType type)
{
  for (Components::Component* component : components)
  {
    if (component->getType() == type)
    {
      return true;
    }
  }
  return false;
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
