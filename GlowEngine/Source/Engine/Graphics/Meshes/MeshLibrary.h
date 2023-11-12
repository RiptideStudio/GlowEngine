/*
/
// filename: MeshLibrary.h
// author: Callen Betts
// brief: defines MeshLibrary class
/
*/

#pragma once

namespace Meshes
{

  class Mesh; // forward declare

  class MeshLibrary
  {

  public:

    // add a new mesh to the library
    void add(Meshes::Mesh* mesh);
    // get a mesh
    Meshes::Mesh* get(std::string name);

    // load all of our preset meshes (quad, etc
    void load();

  private:

    std::map<std::string, Meshes::Mesh*> meshes;

  };

}