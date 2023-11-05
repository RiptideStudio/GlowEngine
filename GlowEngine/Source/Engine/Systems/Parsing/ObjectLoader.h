/*
/
// filename: ObjectLoader.h
// author: Callen Betts
// brief: defines ObjectLoader class
//
// description: The goal here is to create an instance of the object loader and then parse it
//  this way we can just use methods to return the data parsed so we never have to directly look at it
/
*/

#pragma once

namespace Parse
{

  class ObjectLoader
  {

  public:

    ObjectLoader();

    bool open(const std::string fileName);
    void parse(); // parse data and save to indices/vertices
    void close();

    // get the indices
    const std::vector<Index>& getIndices();
    // get the vertex indices
    const std::vector<unsigned short>& getVertexIndices();
    // get the vertices
    const std::vector<Vertex>& getVertices();

    // get the filename
    const std::string getFileName();

  private:
    std::string fileName;
    std::ifstream file;

    std::vector<Vertex> vertices;
    std::vector<Index> indices; // this contains all the index data for normals, vertices and textures
    std::vector<unsigned short> vertexIndices; // this is vertex indices only
    std::vector<Vector3D> vertexNormals; // this is normals for the vertex only
    
  };

}