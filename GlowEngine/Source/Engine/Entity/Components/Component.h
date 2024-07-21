/*
/
// filename: Component.h
// author: Callen Betts
// brief: defines Component class
/
*/

#pragma once
#include "Property.h"

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

      // get our name
      std::string getName() { return name; }
      // get the parent
      Entities::Entity* getParent();
      // set the parent entity
      void setParent(Entities::Entity* newParent);
      // inspect in the editor
      virtual void inspect() {};

      Entities::Entity* parent;

      // add a variable to be modified in the editor
      void AddVariable(Variable var) { variables.push_back(var); }

      // get the variables vector
      std::vector<Variable>& getVariables() { return variables; }

    protected:
      
      // component identifiers
      std::string name;
      // type of component
      ComponentType type;
      // if this component should be updated
      bool active = true;
      // priority determines order of update
      int priority = 0;

      // components have a list of variables we expose to the editor
      std::vector<Variable> variables;
      
  };

}