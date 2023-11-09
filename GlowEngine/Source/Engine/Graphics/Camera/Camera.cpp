/*
/
// filename: Camera.cpp
// author: Callen Betts
// brief: implements Camera.h
/
*/

#include "stdafx.h"
#include "Camera.h"
#include "Engine/GlowEngine.h"
#include "Engine/Graphics/Renderer.h"

// initialize the position and coordinate system of camera
Visual::Camera::Camera(Graphics::Renderer* renderEngine)
  :
  fov(250.f),
  yaw(0),
  pitch(0),
  cameraSpeed(20),
  mouseSensitivity(0.1f)
{
  position = { 0 };
  rotation = 0;
  viewMatrix = {};
  perspectiveMatrix = {};
  target = {0,0,1};
  upDirection = { 0,-1,0 };
  right = { 0,0,0 };
  forward = { 0,0,1 };
  viewDistance = 250;
  windowHeight = 720;
  windowWidth = 1280;
  aspectRatio = windowWidth / windowHeight;
  engine = EngineInstance::getEngine();
  renderer = renderEngine;
  windowHandle = engine->getWindowHandle();
  input = engine->getInputSystem();
}

// update the camera - this is essentially the camera controller
void Visual::Camera::update()
{
  // stop camera functionality if we are not focused in the window
  if (!input->isFocused())
  {
    return;
  }

  // update our camera controller - this handles input and calculates our forward direction, right, and target
  cameraController();

  // Determine the target point based on the new orientation
  target = DirectX::XMVectorAdd(position, forward);

  // Update the camera's up vector
  upDirection = DirectX::XMVector3Cross(forward, right);
  upDirection = DirectX::XMVector3Normalize(upDirection);

  // Update the perspective matrix
  perspectiveMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, 0.1f, viewDistance);

  // Update the view matrix
  viewMatrix = DirectX::XMMatrixLookAtLH(position, target, upDirection);

  // Update the renderer's view and perspective matrices
  renderer->updateConstantBufferCameraMatrices();
}

// controls our camera using input - this is currently debug, as we don't have a player
// class yet
void Visual::Camera::cameraController()
{
  // multiply our camera speed by delta time
  float camSpd = cameraSpeed * engine->getDeltaTime();

  // get the mouse delta so we can look around
  float deltaX = input->getMouseDelta().x;
  float deltaY = input->getMouseDelta().y;

  // calculate the pitch and yaw - this determines how our camera rotates
  yaw -= deltaX * mouseSensitivity;
  pitch -= deltaY * mouseSensitivity;

  // make sure we cannot look behind us
  pitch = max(-89.0f, min(89.0f, pitch));

  // convert the yaw and pitch to a direction vector
  forward = DirectX::XMVectorSet(
    cos(DirectX::XMConvertToRadians(yaw)) * cos(DirectX::XMConvertToRadians(pitch)),
    sin(DirectX::XMConvertToRadians(pitch)),
    sin(DirectX::XMConvertToRadians(yaw)) * cos(DirectX::XMConvertToRadians(pitch)),
    0.0f);

  // calculate the forward direction - this is where our camera is "looking"
  forward = DirectX::XMVector3Normalize(forward);

  // calculate the right vector from us using the global up direction (0,-1,0)
  right = DirectX::XMVector3Cross({ 0,-1,0 }, forward);
  right = DirectX::XMVector3Normalize(right);

  // keyboard input for debug movement
  if (input->keyDown('W'))
  {
    position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(forward, camSpd));
  }
  if (input->keyDown('S'))
  {
    position = DirectX::XMVectorSubtract(position, DirectX::XMVectorScale(forward, camSpd));
  }
  if (input->keyDown('A'))
  {
    position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(right, camSpd));
  }
  if (input->keyDown('D'))
  {
    position = DirectX::XMVectorSubtract(position, DirectX::XMVectorScale(right, camSpd));
  }
  if (input->keyDown(VK_SPACE))
  {
    position = DirectX::XMVectorAdd(position, { 0, camSpd,0 });
  }
  if (input->keyDown(VK_SHIFT))
  {
    position = DirectX::XMVectorAdd(position, { 0, -camSpd,0 });
  }
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
