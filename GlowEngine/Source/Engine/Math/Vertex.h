/*
/
// filename: Vertex.h
// author: Callen Betts
// brief: defines Vertex class
/
*/

#pragma once

namespace GlowMath
{

  // define vertex struct
  struct Vertex
  {
    float x, y, z; // vertices
    float r, g, b, a; // color
    float nx, ny, nz; // normals
  };

  // define index struct
  struct Index 
  {
    int vertexIndex; // this is the index of each vertex
    int textureIndex; // texture coordinate index
    int normalIndex; // normal vertex index
  };

}