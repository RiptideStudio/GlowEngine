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
  class Vector3D
  {

  public:

    // default of zeroed vector
    Vector3D(float x = 0, float y = 0, float z = 0);

    // plus equals operator
    void operator+=(const Vector3D& other);
    void operator-=(const Vector3D& other);

    // plus operator
    Vector3D operator+(const Vector3D& other);
    Vector3D operator-(const Vector3D& other);

    // interpolation
    void lerpTowards(const Vector3D& targetVector, float t = .15f);

    // normalize
    void normalize();
    float dot(const Vector3D other);

    // multiply operator
    Vector3D operator*(const Vector3D& other);
    Vector3D operator*(const float& other);

  public:

    // client can change vector properties
    float x, y, z;

  };

  // typdefs for better readability
  typedef DirectX::XMVECTOR XMVector;
  typedef DirectX::XMMATRIX Matrix;
  typedef GlowMath::Vector3D Vector3D;

}