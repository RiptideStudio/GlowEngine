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

Components::BoxCollider::BoxCollider(Vector3D newScale)
{
  scale = newScale;
  type = ComponentType::Collider;
}

bool Components::BoxCollider::isColliding(const Components::Collider* other)
{
  Vector3D position = parent->transform->getPosition();
  Vector3D otherPosition = other->parent->transform->getPosition();
  const BoxCollider* otherBox = dynamic_cast<const BoxCollider*>(other);

  // Calculate min and max extents for this box
  Vector3D minA = position - scale * 0.5f;
  Vector3D maxA = position + scale * 0.5f;

  // Calculate min and max extents for the other box
  Vector3D otherScale = otherBox->scale;
  Vector3D minB = otherPosition - otherScale * 0.5f;
  Vector3D maxB = otherPosition + otherScale * 0.5f;

  // Add a small tolerance to handle floating-point inaccuracies
  float tolerance = 0.0001f;

  // Check for overlap on the x, y, and z axes
  bool overlapX = (minA.x < maxB.x + tolerance) && (maxA.x > minB.x - tolerance);
  bool overlapY = (minA.y < maxB.y + tolerance) && (maxA.y > minB.y - tolerance);
  bool overlapZ = (minA.z < maxB.z + tolerance) && (maxA.z > minB.z - tolerance);

  // The boxes are colliding if they overlap on all three axes
  return overlapX && overlapY && overlapZ;
}

void Components::BoxCollider::onFirstCollide()
{
  Logger::write("Collided First");
}

void Components::BoxCollider::onCollide()
{
  // when we hit something, this is triggered once
  if (!collided)
  {
    onFirstCollide();
    collided = true;
  }

  // while we are colliding
  Components::Physics* physics = parent->physics;

  physics->setVelocity(0);
  physics->setAcceleration(0);
}

void Components::BoxCollider::onLeaveCollide()
{
  Logger::write("Collider left");
}

void Components::BoxCollider::update()
{
  if (autoSize)
  {
    scale = parent->transform->getScale();
  }
}


