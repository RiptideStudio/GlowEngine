/*
/
// filename: Component.h
// author: Callen Betts
// brief: defines Component class
/
*/

#pragma once

namespace Entities
{
  class Entity;
}

namespace Components
{


  class Component
  {
    public:

    enum ComponentType
    {
      Transform,
      Behavior,
      Sprite2D,
      Sprite3D,
      Animation3D,
      BoxCollider,
      Physics,
      Collider,
      None
    };

    public:
      Component();

      virtual void update() {};
      virtual void render() {};
      virtual void exit() {};
      virtual void load(const nlohmann::json&) {};

      virtual Components::Component* clone();

      int getPriority() { return priority; }

      const ComponentType getType();

      // get the parent
      Entities::Entity* getParent();
      // set the parent entity
      void setParent(Entities::Entity* newParent);

      Entities::Entity* parent;

    protected:
      
      // component identifiers
      std::string name;
      ComponentType type;
      // if this component should be updated
      bool active = false;
      // priority determines order of update
      int priority = 0;
      
  };

}