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
      None
    };

    public:
      Component();

      virtual void update() {};
      virtual void render() {};
      virtual void exit() {};

      const ComponentType getType();

      // get the parent
      Entities::Entity* getParent();
      // set the parent entity
      void setParent(Entities::Entity* newParent);

    protected:
      
      Entities::Entity* parent;
      std::string name;
      ComponentType type;
      
    private:

  };

}