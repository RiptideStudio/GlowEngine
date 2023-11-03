/*
/
// filename: Camera.cpp
// author: Callen Betts
// brief: implements Camera.h
/
*/

#include "stdafx.h"
#include "Camera.h"

// initialize the position and coordinate system of camera
Visual::Camera::Camera(HWND handle)
  :
  windowHandle(handle),
  fov(10.f)
{
  position = { 0 };
  rotation = 0;
  viewMatrix = {};
  perspectiveMatrix = {};
  target = {};
  upDirection = { 0,1,0 };
  viewDistance = 500; // default 500 view distance
  windowHeight = 720;
  windowWidth = 1280;
  aspectRatio = windowWidth / windowHeight;
}

// update the camera - this is essentially the camera controller
void Visual::Camera::update()
{
  
  // determine the target point (where we are looking)
  target = DirectX::XMVectorAdd(position, { 0,0,1,0 });

  // get the right, left, and up directions of the camera
  XMVector forward = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(target, position));
  XMVector right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(upDirection, forward));
  upDirection = DirectX::XMVector3Cross(forward, right);

  // update the perspective matrix
  perspectiveMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, 0.01f, viewDistance);

  // transformation
  viewMatrix = DirectX::XMMatrixLookAtLH(position, target, upDirection);
}

// get the view matrix
const Matrix& Visual::Camera::getViewMatrix()
{
  return viewMatrix;
}

// get the perspective matrix
const Matrix& Visual::Camera::getPerspecitveMatrix()
{
  return perspectiveMatrix;
}
