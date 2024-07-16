/*
/
// filename: PlayerBehavior.cpp
// author: Callen Betts
// brief: implements PlayerBehavior.h
/
*/

#include "stdafx.h"
#include "PlayerBehavior.h"
#include "Engine/Graphics/Camera/Camera.h"
#include "Engine/EngineInstance.h"
#include "Engine/GlowEngine.h"

// create a PlayerBehavior
Game::PlayerBehavior::PlayerBehavior()
  : Behavior()
{
  type = Components::Component::ComponentType::Behavior;
  moveSpeed = 20.f;
  jumpSpeed = 20.f;
  god = false;
}

// move our player
void Game::PlayerBehavior::update()
{
  // get the components we need to move
  Components::Transform* transform = getComponentOfType(Transform, parent);
  Components::Physics* physics = getComponentOfType(Physics, parent);

  if (!physics || !transform)
    return;

  // moves based on the direction we're looking
  Visual::Camera* camera = engine->getCamera();
  Vector3D forward = camera->getForwardVector();
  Vector3D right = camera->getRightVector();

  forward.y = 0;
  forward.normalize();

  camera->setTarget(parent);

  if (input->keyDown('W'))
  {
    physics->setVelocity(forward * moveSpeed);
  }
  if (input->keyDown('A'))
  {
    physics->setVelocity(right * moveSpeed);
  }
  if (input->keyDown('S'))
  {
    physics->setVelocity(forward * -moveSpeed);
  }
  if (input->keyDown('D'))
  {
    physics->setVelocity(right * -moveSpeed);
  }

  // jump
  if (input->keyTriggered(VK_SPACE) && !god)
  {
    physics->setVelocityY(jumpSpeed);
  }

  // godmode lets us fly unbound by physics and collision!
  if (god)
  {
    if (input->keyDown(VK_SPACE))
    {
      physics->setVelocityY(moveSpeed);
    }
    if (input->keyDown(VK_SHIFT))
    {
      physics->setVelocityY(-moveSpeed);
    }
  }
}

