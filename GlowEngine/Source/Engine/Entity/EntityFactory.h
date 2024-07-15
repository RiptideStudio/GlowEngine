/*
/
// filename: EntityFactory.h
// author: Callen Betts
// brief: defines base EntityFactory class
/
*/

#pragma once

#include "Components/Component.h"

namespace Entities
{
  class EntityFactory : public Systems::System
  {

  public:

    // initialize as a system with core pointers
    EntityFactory(std::string directoryPath = "Data/Entities");

    // add an entity to the archetype map
    void addArchetype(std::string name, std::string filePath);

    // load an entity's data
    Entities::Entity* loadEntity(std::string name);

    // create an entity and add it to the active scene (using loadEntity())
    Entities::Actor* createEntity(std::string name, Vector3D position);

  private:

    // map of names to entity archetypes
    std::map<std::string, Entities::Entity*> archetypes;
  };
}