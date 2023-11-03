/*
/
// filename: GlowMath.h
// author: Callen Betts
// brief: defines GlowMath class
/
*/

#pragma once

namespace GlowMath
{

  // vector
  struct Vector3D
  {
    float x, y, z;
  };


  // typdefs for better readability
  typedef DirectX::XMVECTOR XMVector;
  typedef DirectX::XMMATRIX Matrix;
  typedef GlowMath::Vector3D Vector3D;


}