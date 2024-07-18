/*
/
// filename: Physics.cpp
// author: Callen Betts
// brief: implements Physics.h
/
*/

#include "stdafx.h"
#include "BoxCollider.h"
#include "Engine/GlowEngine.h"
#include "Engine/Graphics/Renderer.h"
#include "Engine/Graphics/Meshes/MeshLibrary.h"

Components::BoxCollider::BoxCollider(Vector3D newScale)
{
  scale = newScale;
  type = ComponentType::Collider;
}

// return whether or not two colliders are colliding
bool Components::BoxCollider::isColliding(const Components::Collider* other)
{
  Vector3D position = parent->transform->getPosition();
  Vector3D otherPosition = other->parent->transform->getPosition();
  const BoxCollider* otherBox = dynamic_cast<const BoxCollider*>(other);

  return isAABBColliding(*otherBox);
}

bool Components::BoxCollider::isAABBColliding(const BoxCollider& other) 
{
  Vector3D position = parent->transform->getPosition();
  Vector3D otherPosition = other.parent->transform->getPosition();

  Vector3D minA = position - scale * 0.5f;
  Vector3D maxA = position + scale * 0.5f;

  Vector3D otherScale = other.scale;
  Vector3D minB = otherPosition - otherScale * 0.5f;
  Vector3D maxB = (otherPosition + otherScale * 0.5f);

  bool overlapX = (minA.x < maxB.x) && (maxA.x > minB.x);
  bool overlapY = (minA.y <= maxB.y) && (maxA.y > minB.y);
  bool overlapZ = (minA.z < maxB.z) && (maxA.z > minB.z);

  return overlapX && overlapY && overlapZ;
}

void Components::BoxCollider::onCollide(const Components::Collider* other)
{
  // Get the physics and transform components
  Components::Physics* physics = getComponentOfType(Physics, parent);
  Components::Transform* transform = getComponentOfType(Transform, parent);
  const BoxCollider* otherBox = dynamic_cast<const BoxCollider*>(other);

  // Get the current position and velocity
  Vector3D currentPosition = transform->getPosition();
  Vector3D oldPosition = transform->getOldPosition();
  Vector3D velocity = physics->getVelocity();

  // Get the positions of both colliding objects
  Vector3D otherPosition = other->parent->transform->getPosition();

  // Calculate the half-sizes
  Vector3D halfSizeA = scale * 0.5f;
  Vector3D otherScale = otherBox->scale;
  Vector3D halfSizeB = otherScale * 0.5f;

  // Calculate the penetration depths along each axis
  float penetrationX = halfSizeA.x + halfSizeB.x - std::abs(currentPosition.x - otherPosition.x);
  float penetrationY = halfSizeA.y + halfSizeB.y - std::abs(currentPosition.y - otherPosition.y);
  float penetrationZ = halfSizeA.z + halfSizeB.z - std::abs(currentPosition.z - otherPosition.z);

  // Determine the primary collision axis by finding the smallest penetration depth
  float minPenetration = penetrationX;
  Vector3D collisionNormal = Vector3D(1, 0, 0);

  if (penetrationY < minPenetration)
  {
    collisionNormal = Vector3D(0, 1, 0); // Y-axis
    minPenetration = penetrationY;
  }

  if (penetrationZ < minPenetration)
  {
    collisionNormal = Vector3D(0, 0, 1); // Z-axis
    minPenetration = penetrationZ;
  }

  if (minPenetration <= 0) 
    return;

  // Adjust the velocity based on the collision normal
  velocity = velocity - collisionNormal * (velocity.dot(collisionNormal));

  // If the smallest penetration is in the Y-axis, handle grounding and position adjustment
  if (collisionNormal.y == 1 && currentPosition.y > otherPosition.y)
  {
    physics->setGrounded(true);
    velocity.y = 0;

    if (minPenetration > 0)
    {
      currentPosition.y += minPenetration * 0.5f; // Adjust slightly to avoid jitter
      transform->setPosition(currentPosition);
    }
  }
  else if (!physics->isAnchored())
  {
    if (minPenetration > 0)
    {
      currentPosition += collisionNormal * minPenetration * 0.00001f;
      transform->setPosition({oldPosition.x,currentPosition.y,oldPosition.z});
    }
  }

  // Set the adjusted velocity
  physics->setVelocity(velocity);
}

void Components::BoxCollider::setHitboxSize(Vector3D hitboxSize)
{
  calculateScale();
  meshScale = hitboxSize / parent->transform->getScale();
  scale = hitboxSize;
  dirty = false;
}

// draw a wireframe of the box based on vertices
void Components::BoxCollider::renderDebug()
{
  Graphics::Renderer* renderer = EngineInstance::getEngine()->getRenderer();
  Meshes::MeshLibrary* lib = EngineInstance::getEngine()->getMeshLibrary();

  renderer->drawSetColor(Color::Red);
  lib->drawBox(this);
  renderer->drawSetColor(Color::Clear);
}