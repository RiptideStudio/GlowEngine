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
  fov(100.f)
{
  position = { 0 };
  rotation = 0;
  viewMatrix = {};
  perspectiveMatrix = {};
  target = {};
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
  float camSpd = 10.f*engine->getDeltaTime();
  // debug
  Engine::GlowEngine* engine = EngineInstance::getEngine();
  Input::InputSystem* input = engine->getInputSystem();

  if (input->keyDown('E'))
  {
    target = DirectX::XMVectorAdd(target, { camSpd,0,0 });
  }
  if (input->keyDown('A'))
  {
    position = DirectX::XMVectorAdd(position, { -camSpd,0,0 });
  }
  if (input->keyDown('D'))
  {
    position = DirectX::XMVectorAdd(position, { camSpd,0,0 });
  }
  if (input->keyDown('W'))
  {
    position = DirectX::XMVectorAdd(position, { 0,0,camSpd });
  }
  if (input->keyDown('S'))
  {
    position = DirectX::XMVectorAdd(position, { 0,0,-camSpd });
  }
  if (input->keyDown(VK_SPACE))
  {
    position = DirectX::XMVectorAdd(position, { 0, camSpd,0 });
  }
  if (input->keyDown(VK_SHIFT))
  {
    position = DirectX::XMVectorAdd(position, { 0, -camSpd,0 });
  }
  // determine the target point (where we are looking)
  target = DirectX::XMVectorAdd(position, { 0,0,1,0 });

  // get the right, left, and up directions of the camera
  XMVector forward = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(target, position));
  XMVector right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(upDirection, forward));
  upDirection = DirectX::XMVector3Cross(forward, right);

  // update the perspective matrix
  perspectiveMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, 0.1f, viewDistance);

  // transformation
  viewMatrix = DirectX::XMMatrixLookAtLH(position, target, upDirection);

  // update the renderer's view and perspective matrices
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
