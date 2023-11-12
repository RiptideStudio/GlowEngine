/*
/
// filename: Mesh.h
// author: Callen Betts
// brief: defines Mesh class
/
*/

#pragma once

namespace Meshes
{
  class MeshLibrary; // forward declare

  class Mesh
  {

  public:

    // construct a new mesh
    Mesh(std::string name = "Quad");

    // initialize a new mesh
    void init();

    // set the mesh's vertices
    void setVertices(const std::vector<Vertex>& vertexList);
    // set the mesh's indices
    void setIndices(std::vector<unsigned short> indexList);

    // get the mesh's data for read only
    const std::vector<Vertex>& getVertices() { return vertices; }
    const std::vector<unsigned short>& getIndices() { return indices; }

    // get the name
    std::string getName() { return name; }

  private:

    Meshes::MeshLibrary* library;

    std::string name;

    std::vector<Vertex> vertices;
    std::vector<unsigned short> indices;

  };

}