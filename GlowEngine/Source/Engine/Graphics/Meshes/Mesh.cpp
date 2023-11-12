/*
/
// filename: Mesh.cpp
// author: Callen Betts
// brief: implements Mesh.h
/
*/

#include "stdafx.h"
#include "Mesh.h"
#include "Engine/GlowEngine.h"
#include "Engine/Graphics/Meshes/MeshLibrary.h"

// create a mesh
Meshes::Mesh::Mesh(std::string meshName)
{
  init();
  name = meshName;
  library->add(this);
}

// initialize a mesh's data
void Meshes::Mesh::init()
{
  library = EngineInstance::getEngine()->getMeshLibrary();
}

// set a mesh's vertices
void Meshes::Mesh::setVertices(const std::vector<Vertex>& vertexList)
{
  for (auto v : vertexList)
  {
    vertices.push_back(v);
  }
}

// set a mesh's indices
void Meshes::Mesh::setIndices(std::vector<unsigned short> indexList)
{
  for (auto i : indexList)
  {
    indices.push_back(i);
  }
}
