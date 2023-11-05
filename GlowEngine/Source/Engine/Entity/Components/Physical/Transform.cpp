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
  dirty(true)
{
  transformMatrix = {};
  type = ComponentType::transform;
}

Components::Transform::Transform(Vector3D pos_, Vector3D scale_, Vector3D rotation_)
  :
  position(pos_),
  scale(scale_),
  rotation(rotation_),
  dirty(true)
{
  transformMatrix = {};
  type = ComponentType::transform;
}

// reclaculate the world matrix
void Components::Transform::recalculateMatrix()
{
  // rotation matrix
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

// check if the matrix needs to be updated
void Components::Transform::update()
{
  // check for recalculating matrix
  if (dirty)
  {
    recalculateMatrix();
  }
  if (EngineInstance::getEngine()->getInputSystem()->keyDown('R'))
  {
    rotation.x += 0.0008f;
    rotation.z += 0.0008f;
    dirty = true;
  }
}

// set the position
void Components::Transform::setPosition(Vector3D pos)
{
  position = pos;
}

