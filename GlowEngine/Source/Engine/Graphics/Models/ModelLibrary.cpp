/*
/
// filename: ModelLibrary.cpp
// author: Callen Betts
// brief: implements ModelLibrary.h
/
*/

#include "stdafx.h"
#include "ModelLibrary.h"
#include "Engine/Systems/Parsing/ObjectLoader.h"

Models::ModelLibrary::ModelLibrary()
{
}

// add preset models (cube, sphere, cylinder, etc)
void Models::ModelLibrary::init()
{
  add("Cube", new Models::Model("Data/Models/Cube/Cube.obj"));
  add("Plane", new Models::Model("Data/Models/Plane/Plane.obj"));
  add("Cylinder", new Models::Model("Data/Models/Cylinder/Cylinder.obj"));
  add("IcoSphere", new Models::Model("Data/Models/IcoSphere/IcoSphere.obj"));
  add("Monkey", new Models::Model("Data/Models/Objects/Monkey.obj"));
  add("Tree", new Models::Model("Data/Models/Objects/Tree.obj"));
}

// add a model to the library
void Models::ModelLibrary::add(std::string name, Models::Model* model)
{
  if (model)
  {
    models[name] = model;
  }
  Logger::write("Added new model " + name + " to library");
}

// get a model from the library
Models::Model* Models::ModelLibrary::get(std::string name)
{
  Models::Model* model = models[name];
  return model;
}
