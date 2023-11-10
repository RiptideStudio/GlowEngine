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
  fileName("none"),
  objects(0)
{
}

// replace a filename's extension
static std::string replaceFileExtension(const std::string& filename, const std::string& newExtension) 
{
  std::string newFilename = filename;
  size_t pos = newFilename.rfind(".obj");
  if (pos != std::string::npos) { // Check if the .obj extension is found
    newFilename.replace(pos, 4, newExtension); // Replace .obj with new extension
  }
  return newFilename;
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

  std::string currentObjectName = "default"; // this will store the name of each object

  // loop through each line within the file and check for a prefix of data
  std::string line;
  while (std::getline(file, line)) 
  {
    // get the data prefix so we know type of data to read
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;

    // get the name of each object 
    if (prefix == "o" || prefix == "g") 
    {
      // read the new object name and use it for subsequent data
      iss >> currentObjectName;
      modelNames.push_back(currentObjectName);
      objects++;
    }
    else if (prefix == "v") 
    {
      // load each vertex
      Vertex vertex{ 0,0,0,1,1,1,1 };
      iss >> vertex.x >> vertex.y >> vertex.z;

      modelVertices[currentObjectName].push_back(vertex); // add the vertex to object map
      vertices.push_back(vertex);
    }
    else if (prefix == "vt")
    {
      // load texture coordinates
      Vector3D uv;
      iss >> uv.x >> uv.y;
      temp_uvs.push_back(uv);
    }
    else if (prefix == "vn")
    {
      // vertex normals
      Vector3D normal;
      iss >> normal.x >> normal.y >> normal.z;
      temp_normals.push_back(normal);
    }
    else if (prefix == "f") 
    {
      // treat each new object
      std::string vertexData;
      while (iss >> vertexData) 
      {
        std::istringstream vertexStream(vertexData);
        int posIndex, texIndex = -1, normIndex = -1;
        char slash;

        vertexStream >> posIndex; // Read the position index
        if (vertexStream >> slash) vertexStream >> texIndex; // Optionally read texture coordinate index
        if (vertexStream >> slash) vertexStream >> normIndex; // Optionally read normal index

        // Adjust for OBJ's 1-based indexing
        posIndex--;
        if (texIndex > 0) texIndex--;
        if (normIndex > 0) normIndex--;

        // Construct a complete vertex with position, normal, and texCoord if available
        Vertex completeVertex = vertices[posIndex]; // Use local vertex data
        if (texIndex >= 0 && texIndex < temp_uvs.size()) 
        {
          Vector3D uv = temp_uvs[texIndex];
          completeVertex.tx = uv.x;
          completeVertex.ty = uv.y;
        }
        if (normIndex >= 0 && normIndex < temp_normals.size()) 
        {
          Vector3D normal = temp_normals[normIndex];
          completeVertex.nx = normal.x;
          completeVertex.ny = normal.y;
          completeVertex.nz = normal.z;
        }

        // Add the complete vertex to the global model vertices for the current object
        modelVertices[currentObjectName].push_back(completeVertex);
        // Add the index of the complete vertex to the global model indices for the current object
        modelIndices[currentObjectName].push_back(modelVertices[currentObjectName].size() - 1);
      }
    }
  }

  // parse the mtl parts of the file
  parseMTL();

  // close the file
  close();
}

// parse MTL data - this contains things like texture names
void Parse::ObjectLoader::parseMTL()
{
  std::string filePath = replaceFileExtension(fileName, ".mtl");

  std::ifstream mtl(filePath);

  bool open = mtl.is_open();
  // open the mtl file
  if (open)
  {
    std::string line;
    std::string currentMaterialName;

    while (std::getline(mtl, line))
    {
      std::istringstream lineStream(line);
      std::string prefix;
      lineStream >> prefix;

      if (prefix == "newmtl")
      {
        lineStream >> currentMaterialName;
      }
      else if (prefix == "map_Kd")
      {
        std::string textureFilePath;
        lineStream >> textureFilePath;

        // Extract the file name from the full file path
        size_t lastSlash = textureFilePath.find_last_of("/\\"); // Handles both forward and backslashes
        std::string textureFileName = textureFilePath.substr(lastSlash + 1);

        // Remove the extension from the texture file name
        size_t lastDot = textureFileName.rfind('.');
        if (lastDot != std::string::npos)
        {
          // Erase the extension part
          textureFileName.erase(lastDot);
        }

        // Save the texture file name without the path and extension
        textureNames.push_back(textureFileName);
      }
    }
  }
  else
  {
    // failed to open
    return;
  }

  // match the model's object data order
  std::reverse(textureNames.begin(), textureNames.end());
}

// delete the parser and its data from memory once we're finished
void Parse::ObjectLoader::close()
{
  if (file.is_open())
  {
    file.close();
  }
}

// kill ourselves
void Parse::ObjectLoader::destroy()
{
  delete this;
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

