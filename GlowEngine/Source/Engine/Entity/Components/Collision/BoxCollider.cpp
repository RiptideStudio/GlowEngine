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
#include "Engine/Graphics/Meshes/Mesh.h"
#include "Engine/Graphics/Models/Model.h"

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

// Cast a ray towards a direction 
bool Components::BoxCollider::rayIntersects(const Vector3D& rayOrigin, const Vector3D& rayDirection, float& tmin, float& tmax) 
{
  Vector3D position = parent->transform->getPosition();
  Vector3D min = position - scale * 0.5f;
  Vector3D max = position + scale * 0.5f;

  tmin = (min.x - rayOrigin.x) / rayDirection.x;
  tmax = (max.x - rayOrigin.x) / rayDirection.x;

  if (tmin > tmax) std::swap(tmin, tmax);

  float tymin = (min.y - rayOrigin.y) / rayDirection.y;
  float tymax = (max.y - rayOrigin.y) / rayDirection.y;

  if (tymin > tymax) std::swap(tymin, tymax);

  if ((tmin > tymax) || (tymin > tmax)) return false;

  if (tymin > tmin) tmin = tymin;
  if (tymax < tmax) tmax = tymax;

  float tzmin = (min.z - rayOrigin.z) / rayDirection.z;
  float tzmax = (max.z - rayOrigin.z) / rayDirection.z;

  if (tzmin > tzmax) std::swap(tzmin, tzmax);

  if ((tmin > tzmax) || (tzmin > tmax)) return false;

  if (tzmin > tmin) tmin = tzmin;
  if (tzmax < tmax) tmax = tzmax;

  return true;
}

// AABB collision detection
bool Components::BoxCollider::isAABBColliding(const BoxCollider& other) 
{
  Vector3D position = parent->transform->getPosition();
  Vector3D otherPosition = other.parent->transform->getPosition();

  Vector3D minA = position - scale * 0.5f;
  Vector3D maxA = position + scale * 0.5f;

  Vector3D otherScale = other.scale;
  Vector3D minB = otherPosition - otherScale * 0.5f;
  Vector3D maxB = otherPosition + otherScale * 0.5f;

  bool overlapX = (minA.x < maxB.x) && (maxA.x > minB.x);
  bool overlapY = (minA.y < maxB.y) && (maxA.y > minB.y);
  bool overlapZ = (minA.z < maxB.z) && (maxA.z > minB.z);

  return overlapX && overlapY && overlapZ;
}

void Components::BoxCollider::onFirstCollide(const Components::Collider* other)
{
  collided = true;
  collidingObjects.insert(other);
  Logger::write("Collision Entered");
}

void Components::BoxCollider::onCollide(const Components::Collider* other)
{
  // when we hit something, this is triggered once
  if (!collided)
  {
    onFirstCollide(other);
  }
  Logger::write("Colliding");

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
  Vector3D collisionNormal = Vector3D(1, 0, 0); // Default to X-axis

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
    if (minPenetration > 0)
    {
      currentPosition.y += minPenetration * 0.5f; // Adjust slightly to avoid jitter
      transform->setPosition(currentPosition);
    }
  }

  // Set the adjusted velocity
  physics->setVelocity(velocity);

  if (!physics->isAnchored())
  {
    // Adjust the position based on the collision normal and penetration depth
    if (minPenetration == penetrationY && penetrationY > 0)
    {
      currentPosition = oldPosition + collisionNormal * minPenetration * 2.f; // Minimal adjustment to avoid jitter
      transform->setPosition(currentPosition);
    }
    else
    {
      transform->setPosition(oldPosition);
    }
  }
}

void Components::BoxCollider::onLeaveCollide(const Components::Collider* other)
{
  // Remove the object from the set of currently colliding objects
  Logger::write("Left collision");
  collidingObjects.erase(other);
  collided = false;

  Components::Physics* physics = getComponentOfType(Physics, parent);
  if (physics)
  {
    bool grounded = false;
    for (auto* collider : collidingObjects)
    {
      Vector3D collisionNormal = parent->transform->getPosition() - collider->parent->transform->getPosition();
      collisionNormal.normalize();
      if (collisionNormal.y > 0)
      {
        grounded = true;
        break;
      }
    }
    physics->setGrounded(grounded);
  }
}

// recalculate the real scale of the mesh when our transform changes
// this can be modified to only be done if the scale changes in the future
void Components::BoxCollider::update()
{
  if (isDirty)
  {
    isDirty = false;
    calculateScale(parent->sprite->getModel()->getModelVertices(), *parent->transform);
  }
}

// our scalar value works for collisions at small values, but as we have larger and larger objects,
// we need to use the original scale of our mesh instead for accurate collisions
void Components::BoxCollider::calculateScale(const std::map<std::string, std::vector<Vertex>>& modelVertices, const Components::Transform& transform)
{
  // Find the model in the map
  if (parent->sprite->getModel()->getModelNames().empty())
  {
    scale = transform.getScale();
    return;
  }

  std::string modelName = parent->sprite->getModel()->getModelNames()[0];

  auto it = modelVertices.find(modelName);

  const std::vector<Vertex>& vertices = it->second;

  // Initialize min and max with the first vertex
  Vector3D min = Vector3D(vertices[0].x, vertices[0].y, vertices[0].z);
  Vector3D max = Vector3D(vertices[0].x, vertices[0].y, vertices[0].z);

  // Loop through all vertices to find the bounding box
  for (const auto& vertex : vertices) {
    if (vertex.x < min.x) min.x = vertex.x;
    if (vertex.y < min.y) min.y = vertex.y;
    if (vertex.z < min.z) min.z = vertex.z;
    if (vertex.x > max.x) max.x = vertex.x;
    if (vertex.y > max.y) max.y = vertex.y;
    if (vertex.z > max.z) max.z = vertex.z;
  }

  // Calculate the original scale of the mesh
  Vector3D originalScale = max - min;

  // Calculate the scaled size based on the transform's scale
  scale = Vector3D(originalScale.x * transform.getScale().x,
    originalScale.y * transform.getScale().y,
    originalScale.z * transform.getScale().z);

  isDirty = parent->transform->isDirty();
}
