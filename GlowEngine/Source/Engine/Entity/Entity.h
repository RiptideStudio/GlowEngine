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
    void update();
    void addComponent(Components::Component* component);
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