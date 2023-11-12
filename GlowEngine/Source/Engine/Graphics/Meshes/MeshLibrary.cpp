/*
/
// filename: MeshLibrary.cpp
// author: Callen Betts
// brief: implements MeshLibrary.h
/
*/

#include "stdafx.h"
#include "MeshLibrary.h"
#include "Engine/Graphics/Meshes/Mesh.h"

// add a mesh to the library
void Meshes::MeshLibrary::add(Meshes::Mesh* mesh)
{
  if (mesh)
  {
    // add mesh if it was valid
    meshes[mesh->getName()] = mesh;
    Logger::write("Added new mesh "+mesh->getName());
  }
  else
  {
    // invalid mesh
    Logger::error("Failed to add mesh " + mesh->getName() + " to library");
  }
}

// get a mesh given a name
Meshes::Mesh* Meshes::MeshLibrary::get(std::string name)
{
  return meshes[name];
}

// load all of our preset meshes
void Meshes::MeshLibrary::load()
{
  // ** Quad Mesh ** //
  Meshes::Mesh* quadMesh = new Meshes::Mesh("Quad");
  // vertices
  std::vector<Vertex> vertices = 
  {
    // Vertex 1
    { -0.5f, 0, 1,
      1.0f,  1.0f,  1.0f, 1.0f,
      0,1,0,
      1.0f,  0.0f },

    // Vertex 2
    { 1,0,1,
      1.0f,  1.0f,  1.0f, 1.0f,
      0,1,0,
      0.0f,  1.0f },  // Texture coordinates

    // Vertex 3
    { -1,0,-1,
      1.0f,  1.0f,  1.0f, 1.0f,
      0,1,0,
      0.0f,  0.0f },  // Texture coordinates

    // Vertex 4
    { 1,0,-1,
      1.0f,  1.0f,  1.0f, 1.0f,
      0,1,0,
      1.0f,  1.0f }   // Texture coordinates
  };
  quadMesh->setVertices(vertices);
  // indices
  std::vector<unsigned short> indices =
  {
    0,1,2,
    0,2,3
  };
  quadMesh->setIndices(indices);
}
