/*
/
// filename: Entity.h
// author: Callen Betts
// brief: defines base entity class
/
*/

#pragma once

#include "Components/Component.h"
#define getComponentOfType(type, entity) dynamic_cast<Components::type*>(entity->getComponent(Components::Component::ComponentType::type##))

namespace Entities
{

  class Entity
  {

  public:

    Entity();
    virtual ~Entity();

    // update all of an entity's components
    void update();
    // render an entity's components
    void render();
    // add a new component
    void addComponent(Components::Component* component);
    // destroy an entity
    void destroy();
    // return if destroyed
    bool isDestroyed() { return destroyed; }
    // get the name
    std::string getName() { return name; }
    // has a component
    bool hasComponent(Components::Component::ComponentType type);
    // get a component
    Components::Component* getComponent(Components::Component::ComponentType type);

  protected:

    int id;
    bool destroyed;
    std::string name;

    std::vector<Components::Component*> components; // entity component list

  private:

  };

}