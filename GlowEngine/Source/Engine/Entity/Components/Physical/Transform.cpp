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
  position({ 0, 0, 1 }),
  scale({ 1, 1, 1}),
  rotation({ 0,0,0 }),
  dirty(true)
{
  transformMatrix = {};
}

// reclaculate the world matrix
void Components::Transform::recalculateMatrix()
{
  if (dirty)
  {
    Engine::GlowEngine* engine = EngineInstance::getEngine();
    rotation.x += 0.001f;
    rotation.y += 0.001f;
    rotation.z += 0.001f;
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
    // dirty = false;
  }
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

