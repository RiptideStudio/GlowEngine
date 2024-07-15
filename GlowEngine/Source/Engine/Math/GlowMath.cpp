/*
/
// filename: GlowMath.cpp
// author: Callen Betts
// brief: implements GlowMath.h
/
*/

#include "stdafx.h"
#include "GlowMath.h"


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

Vector3D GlowMath::Vector3D::operator*(const Vector3D& other)
{
  return Vector3D(x * other.x, y * other.y, z * other.z);
}

Vector3D GlowMath::Vector3D::operator*(const float& other)
{
  return Vector3D(x * other, y * other, z * other);
}

