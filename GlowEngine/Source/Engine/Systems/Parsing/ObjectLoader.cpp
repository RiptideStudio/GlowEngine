/*
/
// filename: ObjectLoader.cpp
// author: Callen Betts
// brief: implements ObjectLoader.h
/
*/

#include "stdafx.h"
#include "ObjectLoader.h"
#include <sstream>

// default constructor for object loading
Parse::ObjectLoader::ObjectLoader()
  :
  fileName("none")
{
}

// attempt to open a file - this saves file path and file data
bool Parse::ObjectLoader::open(const std::string filePath)
{
  fileName = filePath;
  file = std::ifstream(fileName);

  bool opened = file.is_open();
  
  if (!opened)
  {
    // error opening file
    Logger::write("Failed to open ", fileName);
  }

  return opened;
}

// parse a the file's contents
// this will iterate over the .obj file and save vertices, indices, and other core data
void Parse::ObjectLoader::parse()
{
  std::vector<Vector3D> temp_normals;
  std::vector<Vector3D> temp_uvs;

  // loop through each line within the file and check for a prefix of data
  std::string line;
  while (std::getline(file, line)) 
  {
    // get the data prefix so we know what to read
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;
    static int toggle = 0; // This will alternate between 0 and 1

    // load each vertex
    if (prefix == "v") 
    {
      Vertex vertex;
      vertex.r = 1;
      vertex.g = 1;
      vertex.b = 1;
      vertex.a = 1;
      iss >> vertex.x >> vertex.y >> vertex.z;

      vertices.push_back(vertex);
    }
    // load texture coordinates
    else if (prefix == "vt")
    {
      Vector3D uv;
      iss >> uv.x >> uv.y;
      temp_uvs.push_back(uv);
    }
    // vertex normals
    else if (prefix == "vn")
    {
      Vector3D normal;
      iss >> normal.x >> normal.y >> normal.z;
      temp_normals.push_back(normal);
    }
    // facae indices
    else if (prefix == "f") 
    {
      std::array<int, 3> faceNormalIndices; // store the indices of the normals for each face
      std::vector<Index> faceVertices; // to hold vertices of the current face

      std::string vertexData;
      while (iss >> vertexData) 
      {
        std::replace(vertexData.begin(), vertexData.end(), '/', ' ');
        std::istringstream vertexStream(vertexData);
        Index idx;

        // read the vertex index and normal index. Assume no texture coordinates for simplicity
        vertexStream >> idx.vertexIndex >> idx.textureIndex >> idx.normalIndex;

        // adjust for OBJ's 1-based indexing
        idx.vertexIndex--;
        idx.textureIndex--;
        idx.normalIndex--;

        if (idx.textureIndex >= 0 && idx.textureIndex < temp_uvs.size())
        {
          Vertex vertex = vertices[idx.vertexIndex];
          Vector3D uv = temp_uvs[idx.textureIndex];
          vertex.tx = uv.x;
          vertex.ty = uv.y;
          // Since this vertex now has a unique combination of position, normal, and texcoord,
          // we should add it to the vertices list and use the new index in the index buffer.
          vertices.push_back(vertex);
          idx.vertexIndex = vertices.size() - 1;
        }

        faceVertices.push_back(idx); // add this vertex index to the current face
      }
      // we want flat shading
      if (!faceVertices.empty() && faceVertices[0].normalIndex >= 0) 
      {
        Vector3D faceNormal = temp_normals[faceVertices[0].normalIndex];

        for (const auto& fv : faceVertices) 
        {
          Vertex newVertex = vertices[fv.vertexIndex];
          newVertex.nx = faceNormal.x;
          newVertex.ny = faceNormal.y;
          newVertex.nz = faceNormal.z;
          vertices.push_back(newVertex);
          vertexIndices.push_back(vertices.size() - 1);
        }
      }
    }
  }

  // close the file
  close();
}

// delete the parser and its data from memory once we're finished
void Parse::ObjectLoader::close()
{
  if (file.is_open())
  {
    file.close();
  }
}

// get the index container
const std::vector<Index>& Parse::ObjectLoader::getIndices()
{
  return indices;
}

// get the vertex index container
const std::vector<unsigned short>& Parse::ObjectLoader::getVertexIndices()
{
  return vertexIndices;
}

// get the vertex container
const std::vector<Vertex>& Parse::ObjectLoader::getVertices()
{
  return vertices;
}

// get the file path of the object file
const std::string Parse::ObjectLoader::getFileName()
{
  return fileName;
}

