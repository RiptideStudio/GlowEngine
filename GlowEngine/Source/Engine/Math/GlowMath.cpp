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
#include "Engine/Graphics/Window/Window.h"
#include "Engine/Graphics/Camera/Camera.h"
#include "Engine/GlowEngine.h"

// method to turn screen coordinates into world coordinates; used for selecting objects or interacting with mouse
Vector3D GlowMath::Vector3D::ScreenToWorldCoords(Vector3D coords)
{
  float viewWidth = Graphics::Window::GetWidth();
  float viewHeight = Graphics::Window::GetHeight();

  Visual::Camera* camera = EngineInstance::getEngine()->getCamera();

  DirectX::XMMATRIX viewMatrix = camera->getViewMatrix();
  DirectX::XMMATRIX perspectiveMatrix = camera->getPerspecitveMatrix();

  // Convert screen coordinates to normalized device coordinates (NDC)
  float x = (2.0f * coords.x) / viewWidth - 1.0f;
  float y = 1.0f - (2.0f * coords.y) / viewHeight;
  float z = 2.0f * coords.z - 1.0f;

  // Convert NDC to clip space coordinates
  DirectX::XMVECTOR clipCoords = DirectX::XMVectorSet(x, y, z, 1.0f);

  // Inverse projection transformation
  DirectX::XMVECTOR eyeCoords = DirectX::XMVector4Transform(clipCoords, DirectX::XMMatrixInverse(nullptr, perspectiveMatrix));
  eyeCoords = DirectX::XMVectorSet(DirectX::XMVectorGetX(eyeCoords), DirectX::XMVectorGetY(eyeCoords), -1.0f, 1.0f);

  // Inverse view transformation
  DirectX::XMVECTOR worldCoords = DirectX::XMVector4Transform(eyeCoords, DirectX::XMMatrixInverse(nullptr, viewMatrix));
  worldCoords = DirectX::XMVectorScale(worldCoords, 1.0f / DirectX::XMVectorGetW(worldCoords));

  return Vector3D(DirectX::XMVectorGetX(worldCoords), DirectX::XMVectorGetY(worldCoords), DirectX::XMVectorGetZ(worldCoords));
}

Vector3D GlowMath::Vector3D::XMVectorToVector3D(DirectX::XMVECTOR vector)
{
  return Vector3D(DirectX::XMVectorGetX(vector), DirectX::XMVectorGetY(vector), DirectX::XMVectorGetZ(vector));
}

Vector3D GlowMath::Vector3D::XMFloatToVector3D(DirectX::XMFLOAT3 XMFloat)
{
  return Vector3D(XMFloat.x,XMFloat.y,XMFloat.z);
}

bool GlowMath::Vector3D::RayIntersectsBoundingBox(const Vector3D& rayOrigin, const Vector3D& rayDirection, const Components::BoundingBox& box, float& t)
{
  // Implementation of ray-box intersection test (e.g., using the slab method or any other appropriate method)
  // Update 't' with the distance from the ray origin to the intersection point
  // Return true if the ray intersects the bounding box, false otherwise
  return false; // Placeholder
}

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

void GlowMath::Vector3D::operator-=(const Vector3D& other)
{
  x -= other.x;
  y -= other.y;
  z -= other.z;
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

Vector3D GlowMath::Vector3D::operator/(const Vector3D& other)
{
  return Vector3D(x / other.x, y / other.y, z / other.z);
}

