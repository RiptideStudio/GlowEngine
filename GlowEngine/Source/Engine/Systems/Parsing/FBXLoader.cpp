/*
/
// filename: FBXLoader.cpp
// author: Callen Betts
// brief: implements FBXLoader.h
/
*/

#include "stdafx.h"
#include "FBXLoader.h"

Parse::FBXLoader::FBXLoader()
{
  filePath = "none";
}

// open the file and prepare it for parsing
bool Parse::FBXLoader::open(std::string filePath)
{
  std::ifstream file(filePath);

  // check if a file is opens
  if (file.is_open())
  {
    return true;
  }
  else
  {
    // failed to open file, invalid path
    Logger::error("Could not find FBX " + filePath);
    return false;
  }
}

// load an FBX file and parse its data into the class using Assimp
// this will store 3D animation data such as bone transforms, parent bones and nodes
void Parse::FBXLoader::loadFBX()
{
  Assimp::Importer importer;

  // Load the model and specify some post-processing operations
  const aiScene* scene = importer.ReadFile("Assets/Models/Animations/Cube.fbx",
    aiProcess_Triangulate |          // Convert all meshes to triangles
    aiProcess_FlipUVs |              // Flip the texture coordinates on the y-axis
    aiProcess_CalcTangentSpace |     // Calculate tangents and bitangents
    aiProcess_JoinIdenticalVertices  // Join identical vertices/ optimize indexing
  );

  // Check if the import failed
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    // Error handling
    std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
    return;
  }

  // process each of the mesh's nodes
  processNodes(scene->mRootNode, scene);
}

void Parse::FBXLoader::processNodes(aiNode* node, const aiScene* scene)
{
  // Process all the node's meshes
  for (unsigned int i = 0; i < node->mNumMeshes; i++) 
  {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    // processMesh(mesh, scene); // You would have this function defined to process the mesh
  }

  // Then do the same for each of its children
  for (unsigned int i = 0; i < node->mNumChildren; i++) 
  {
    processNodes(node->mChildren[i], scene);
  }
}
