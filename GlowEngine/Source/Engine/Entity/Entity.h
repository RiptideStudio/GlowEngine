/*
/
// filename: Entity.h
// author: Callen Betts
// brief: defines base entity class
/
*/

#pragma once

#include "Components/Component.h"

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

    // get a component of a certain type
    template <typename T>
    T* getComponent(Components::ComponentType type)
    {
      for (auto component : components)
      {
        if (component->getType() == type)
        {
          return dynamic_cast<T*>(component);
        }
      }
      return nullptr;
    }

  protected:

    int id;
    bool destroyed;
    std::string name;

    std::vector<Components::Component*> components; // entity component list

  private:

  };

}