/*
/
// filename: Actor.h
// author: Callen Betts
// brief: defines Actor class
/
*/

#pragma once
#include "Engine/Entity/Entity.h"
#include "Engine/Graphics/Lighting/LightBuffer.h"

namespace Entities
{
 
  class Actor : public Entity
  {

  public:

    Actor();

    // ** Transform ** //
    void setPosition(Vector3D pos) { transform->setPosition(pos); }
    Vector3D getPosition() { return transform->getPosition(); }

    void setScale(Vector3D newScale) { transform->setScale(newScale); }
    Vector3D getScale() { return transform->getScale(); }

    void setRotation(Vector3D pos) { transform->setRotation(pos); }
    Vector3D getRotation() { return transform->getRotation(); }

    // ** Physics ** //
    void setAcceleraton(Vector3D acc);
    void setVelocity(Vector3D vel);

    // ** Model ** //
    void setModel(std::string name);

    // ** Texture ** //
    void setTexture(std::string name);
    void setTextureRepeat(bool val);

    // ** Lighting ** //
    void setAsPointLight(bool val);
    void createPointLight();
    void updatePointLight(Vector3D pos, float size, DirectX::XMFLOAT4 color);
    void setLightSize(float size);

    // ** General ** //
    void setName(std::string name) { this->name = name; }

  private:

    // lighting properties
    bool isLight;
    PointLight* light;

    // common pointers actors use
    Engine::GlowEngine* engine;
    Graphics::Renderer* renderer;

    // components
    Components::Transform* transform;
    Components::Sprite3D* sprite;
    Components::Physics* physics;

  };

}