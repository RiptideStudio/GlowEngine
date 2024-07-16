/*
/
// filename: GlowMath.cpp
// author: Callen Betts
// brief: implements GlowMath.h
/
*/

#include "stdafx.h"
#include "GlowMath.h"
#include "Lerp.h"

GlowMath::Vector3D::Vector3D(float x, float y, float z)
  :
  x(x),
  y(y),
  z(z)
{
}

void GlowMath::Vector3D::operator+=(const Vector3D& other)
{
  x += other.x;
  y += other.y;
  z += other.z;
}

Vector3D GlowMath::Vector3D::operator+(const Vector3D& other)
{
  return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D GlowMath::Vector3D::operator-(const Vector3D& other)
{
  return Vector3D(x - other.x, y - other.y, z - other.z);
}

void GlowMath::Vector3D::lerpTowards(const Vector3D& targetVector, float t)
{
  x = Lerper::Ease(x, targetVector.x, t);
  y = Lerper::Ease(y, targetVector.y, t);
  z = Lerper::Ease(z, targetVector.z, t);
}

void GlowMath::Vector3D::normalize()
{
  float length = std::sqrt(x * x + y * y + z * z);
  if (length > 0.0f) 
  {  
    x /= length;
    y /= length;
    z /= length;
  }
}

float GlowMath::Vector3D::dot(const Vector3D other)
{
  return x * other.x + y * other.y + z * other.z;
}

Vector3D GlowMath::Vector3D::operator*(const Vector3D& other)
{
  return Vector3D(x * other.x, y * other.y, z * other.z);
}

Vector3D GlowMath::Vector3D::operator*(const float& other)
{
  return Vector3D(x * other, y * other, z * other);
}

