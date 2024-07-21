/*
/
// filename: EntityFactory.cpp
// author: Callen Betts
// brief: implements EntityFactory.h
/
*/

#include "stdafx.h"
#include "EntityFactory.h"
#include "Entity.h"
#include "Game/Scene/Scene.h"
#include "Game/Scene/SceneSystem.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

// initialize the entity factory and load all the archetypes from HJSON data
Entities::EntityFactory::EntityFactory(std::string directoryPath) : System("EntityFactory")
{
  // check if the directory exists and if it is a directory
  if (fs::is_directory(directoryPath) && fs::exists(directoryPath))
  {
    // iterate over each entry
    for (const auto& entry : fs::recursive_directory_iterator(directoryPath))
    {
      if (fs::exists(entry) && fs::is_regular_file(entry))
      {
        // valid file, add it to the library
        std::string fileName = entry.path().stem().string();
        std::string filePath = entry.path().string();
        std::string fileType = entry.path().extension().string();
        // check that the file is a json file
        if (fileType == ".json")
        {
          addArchetype(fileName, filePath);
        }
      }
    }
  }
}

// add a new archetype to the list
void Entities::EntityFactory::addArchetype(std::string name, std::string filePath)
{
  // don't add to the archetype if it doesn't exist (for some reason)
  if (fs::exists(filePath))
  {
    archetypes[name] = loadEntity(filePath);
    Logger::write("Added entity archetype from " + filePath);
  }
}

// loads an entity from a file given a name
Entities::Entity* Entities::EntityFactory::loadEntity(std::string filePath)
{
  // entity to load data into
  Entities::Entity* entity = new Entities::Entity();

  // parse the hjson data
  try 
  {
    json data;
    std::ifstream file(filePath);
    file >> data;
    file.close();

    // call entity load
    entity->load(data);
  }
  catch (const std::exception& e)
  {
    Logger::error("Failed to load " + filePath);
  }

  return entity;
}

// creates an entity given a name
Entities::Entity* Entities::EntityFactory::createEntity(std::string name, Vector3D position, EntityType type)
{
  // if archetype was invalid, return an empty entity
  if (!archetypes[name])
  {
    Logger::error("Failed to create entity " + name);
    return new Entities::Entity();
  }

  // if valid, clone the entity
  Entities::Entity* entity = new Entities::Entity(*archetypes[name]);

  return entity;
}

// creates an actor given a name
Entities::Actor* Entities::EntityFactory::createActor(std::string name, Vector3D position, EntityType type)
{
  // if archetype was invalid, return an empty entity
  if (!archetypes[name])
  {
    Logger::error("Failed to create actor " + name);
    return new Entities::Actor();
  }

  // if valid, clone the entity
  Entities::Actor* entity = new Entities::Actor(*archetypes[name]);

  return entity;
}