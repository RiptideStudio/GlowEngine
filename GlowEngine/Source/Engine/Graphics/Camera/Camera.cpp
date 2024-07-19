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
#include "Engine/Graphics/Window/Window.h"

// initialize the position and coordinate system of camera
Visual::Camera::Camera(Graphics::Renderer* renderEngine)
  :
  fov(200.f),
  yaw(0),
  pitch(0),
  cameraSpeed(20),
  mouseSensitivity(0.1f)
{
  position = { 0 };
  rotation = 0;
  viewMatrix = {};
  perspectiveMatrix = {};
  targetPosition = {0,0,1};
  target = nullptr;
  upDirection = { 0,1,0 };
  right = { 0,0,0 };
  forward = { 0,0,1 };
  viewDistance = 250;
  height = 3; // how high we feel 
  engine = EngineInstance::getEngine();
  renderer = renderEngine;
  windowHandle = engine->getWindowHandle();
  input = engine->getInputSystem();
  window = engine->getWindow();
  windowHeight = window->getHeight();
  windowWidth = window->getWidth();
  aspectRatio = windowWidth / windowHeight;
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

  // update our target position
  if (target)
  {
    Vector3D targetPos = target->transform->getPosition();
    position = DirectX::XMVectorSet(targetPos.x, targetPos.y+height, targetPos.z, 1.0f);
  }

  targetPosition = DirectX::XMVectorAdd(position, forward);

  // Update the camera's up vector
  upDirection = DirectX::XMVector3Cross(forward, right);
  upDirection = DirectX::XMVector3Normalize(upDirection);

  // Update the perspective matrix
  perspectiveMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, 0.1f, viewDistance);

  // Update the view matrix
  viewMatrix = DirectX::XMMatrixLookAtLH(position, targetPosition, upDirection);

  // Update the renderer's view and perspective matrices
  renderer->updateObjectBufferCameraMatrices();
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

const Vector3D Visual::Camera::getForwardVector()
{
  XMVector normalForward = DirectX::XMVector3Normalize(forward);
  return Vector3D(DirectX::XMVectorGetX(normalForward), DirectX::XMVectorGetY(normalForward), DirectX::XMVectorGetZ(normalForward));
}

const Vector3D Visual::Camera::getRightVector()
{
  return Vector3D(DirectX::XMVectorGetX(right), DirectX::XMVectorGetY(right), DirectX::XMVectorGetZ(right));
}

void Visual::Camera::setTarget(Entities::Entity* newTarget)
{
  target = newTarget;
}
