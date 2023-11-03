/*
/
// filename: Model.cpp
// author: Callen Betts
// brief: implements Model.h
/
*/

#include "stdafx.h"
#include "Model.h"
#include <filesystem>

// initialize a new model
Components::Model::Model()
  :
  modelType(ModelType::Cube)
{
  load("Data/Models/Cube/Cube.json");
}

// parse a .obj file and load its vertex data into the model
// will instead load primitive type if object file was invalid
void Components::Model::load(const std::string fileName)
{
  std::ifstream file(fileName);
  nlohmann::json data;
  file >> data;

  if (file.is_open())
  {
    // load the vertices
    for (auto vertex : data["vertices"])
    {
      Vertex v;

      v.x = vertex["v"][0];
      v.y = vertex["v"][1];
      v.z = vertex["v"][2];
      vertices.push_back(v);
    }
    // load the indices
    for (auto index : data["indices"])
    {
      indices.push_back(index);
    }
  }
}

// load a model based on its model type
void Components::Model::loadPrimitive()
{

}

// get vertices contianer
const std::vector<GlowMath::Vertex>& Components::Model::getVerties()
{
  return vertices;
}

// get indices container
const std::vector<unsigned short>& Components::Model::getIndices()
{
  return indices;
}
