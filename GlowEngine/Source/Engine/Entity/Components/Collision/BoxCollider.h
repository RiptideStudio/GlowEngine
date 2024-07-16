/*
/
// filename: BoxCollider.h
// author: Callen Betts
// brief: defines the BoxCollider class, holds vertices and BoxCollider data
/
*/

#pragma once
#include "Engine/Entity/Components/Component.h"
#include "Engine/Entity/Components/Collision/Collider.h"

namespace Models { class Model; }

namespace Components
{

  class BoxCollider : public Collider
  {

  public:

    BoxCollider(Vector3D newScale = {1,1,1});

    virtual bool isColliding(const Components::Collider*) override;

    virtual void onFirstCollide(const Components::Collider* other);
    virtual void onCollide(const Components::Collider* other);
    virtual void onLeaveCollide(const Components::Collider* other);

    virtual void update();

    bool isAABBColliding(const BoxCollider& other);
    bool rayIntersects(const Vector3D& rayOrigin, const Vector3D& rayDirection, float& tmin, float& tmax);
    void calculateScale(const std::map<std::string, std::vector<Vertex>>& modelVertices, const Components::Transform& transform);

  private:

    Vector3D scale;
    Vector3D scaledSize;

  };

}