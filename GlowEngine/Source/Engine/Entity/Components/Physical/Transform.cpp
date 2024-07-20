/*
/
// filename: Transform.cpp
// author: Callen Betts
// brief: implements Transform.h
/
*/

#include "stdafx.h"
#include "Transform.h"
#include "Engine/GlowEngine.h"

// initialize a new Transform
Components::Transform::Transform()
  :
  position({ 0, 0, 10 }),
  scale({ 1, 1, 1}),
  rotation({ 0,0,0 }),
  dirty(true),
  anchored(false)
{
  recalculateMatrix();
  type = ComponentType::Transform;
  name = "Transform";
}

// initialize the transform with position, scale, and rotation
Components::Transform::Transform(Vector3D pos_, Vector3D scale_, Vector3D rotation_)
  :
  position(pos_),
  scale(scale_),
  rotation(rotation_),
  dirty(true),
  anchored(false)
{
  recalculateMatrix();
  type = ComponentType::Transform;
}

// reclaculate the world matrix
void Components::Transform::recalculateMatrix()
{
  // rotation matrix
  rotation.x *= 1 / 52.7f;
  rotation.y *= 1 / 52.7f;
  rotation.z *= 1 / 52.7f;

  Matrix rotationMatrixX = DirectX::XMMatrixRotationX(rotation.x);
  Matrix rotationMatrixY = DirectX::XMMatrixRotationY(rotation.y);
  Matrix rotationMatrixZ = DirectX::XMMatrixRotationZ(rotation.z);
  Matrix rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;

  // scale matrix
  Matrix scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

  // translation matrix
  Matrix translationMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

  // set the transform matrix
  transformMatrix = scaleMatrix * rotationMatrix * translationMatrix;

  // reset dirty flag
  dirty = false;
}

// if matrix is dirty
bool Components::Transform::isDirty()
{
  return dirty;
}

// get the transform matrix
const Matrix& Components::Transform::getTransformMatrix()
{
  return transformMatrix;
}

Components::Transform* Components::Transform::clone()
{
  Transform* transform = new Transform(*this);
  return transform;
}

void Components::Transform::inspect()
{
  ImGui::SliderFloat("X", &position.x, position.x - 10, position.x + 10);
  ImGui::SliderFloat("Y", &position.y, position.y - 10, position.y + 10);
  ImGui::SliderFloat("Z", &position.z, position.z - 10, position.z + 10);
}
