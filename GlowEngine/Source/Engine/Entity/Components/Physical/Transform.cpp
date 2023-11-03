/*
/
// filename: Transform.cpp
// author: Callen Betts
// brief: implements Transform.h
/
*/

#include "stdafx.h"
#include "Transform.h"

// initialize a new Transform
Components::Transform::Transform()
  :
  position({ 0 }),
  scale({ 64 }),
  rotation(0),
  dirty(true)
{
  transformMatrix = {};
}

// reclaculate the world matrix
void Components::Transform::recalculateMatrix()
{
  dirty = false;
}

// if matrix is dirty
bool Components::Transform::isDirty()
{
  return dirty;
}

const Matrix& Components::Transform::getTransformMatrix()
{
  return transformMatrix;
}

