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
  fov(150.f),
  yaw(0),
  pitch(0)
{
  position = { 0 };
  rotation = 0;
  viewMatrix = {};
  perspectiveMatrix = {};
  target = {0,0,1};
  upDirection = { 0,-1,0 };
  viewDistance = 500; // default 500 view distance
  windowHeight = 720;
  windowWidth = 1280;
  aspectRatio = windowWidth / windowHeight;
  engine = EngineInstance::getEngine();
  renderer = renderEngine;
  windowHandle = engine->getWindowHandle();
}

// update the camera - this is essentially the camera controller
void Visual::Camera::update()
{

  float camSpd = 30.f*engine->getDeltaTime();
  // debug
  Engine::GlowEngine* engine = EngineInstance::getEngine();
  Input::InputSystem* input = engine->getInputSystem();

  float deltaX = input->getMouseDelta().x;
  float deltaY = input->getMouseDelta().y;

  float mouseSensitivity = 0.1f;

  // Calculate new yaw and pitch values
  yaw -= deltaX * mouseSensitivity;
  pitch -= deltaY * mouseSensitivity; // Subtracting to invert Y-axis

  // Constrain the pitch to prevent the camera from flipping
  pitch = max(-89.0f, min(89.0f, pitch));

  // Convert yaw and pitch to a direction vector
  DirectX::XMVECTOR forward = DirectX::XMVectorSet(
    cos(DirectX::XMConvertToRadians(yaw)) * cos(DirectX::XMConvertToRadians(pitch)),
    sin(DirectX::XMConvertToRadians(pitch)),
    sin(DirectX::XMConvertToRadians(yaw)) * cos(DirectX::XMConvertToRadians(pitch)),
    0.0f);
  forward = DirectX::XMVector3Normalize(forward);

  // Assuming upDirection is the global up, which is usually (0,1,0) for Y-up coordinate systems.
  DirectX::XMVECTOR right = DirectX::XMVector3Cross({0,-1,0}, forward);
  right = DirectX::XMVector3Normalize(right);

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
