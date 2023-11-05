/*
/
// filename: Sprite3D.cpp
// author: Callen Betts
// brief: implements Sprite3D.h
/
*/

#include "stdafx.h"
#include "Sprite3D.h"
#include "Engine/GlowEngine.h"
#include "Engine/Graphics/Renderer.h"
#include "Engine/Entity/Entity.h"

// overloaded constructor to take in a model
Components::Sprite3D::Sprite3D(const std::string modelName)
  : Component(),
  renderer(nullptr),
  model(nullptr)
{
  setModel(modelName);
  init();
}

// base Sprite3D constructor to give pointers to renderer
Components::Sprite3D::Sprite3D()
  : Component(),
  renderer(nullptr),
  model(nullptr)
{
  init();
}

// initialize the Sprite3D component
void Components::Sprite3D::init()
{
  type = ComponentType::sprite3D;
  name = "Sprite3D";
  Engine::GlowEngine* engine = EngineInstance::getEngine();
  renderer = engine->getRenderer();
}

// render a Sprite3D's model
void Components::Sprite3D::render()
{
  // set transform constant buffer
  Components::Transform* transform = getParent()->getComponent<Components::Transform>(ComponentType::transform);
  if (!transform)
  {
    return;
  }
  
  // update the constant buffer's world matrix
  renderer->updateConstantBufferWorldMatrix(transform->getTransformMatrix());

  // bind the constant buffer and update subresource
  renderer->updateConstantBuffer();

  // render the model
  model->render();
}

// set this model given a name
void Components::Sprite3D::setModel(const std::string modelName)
{
  this->model = new Models::Model(modelName);
}

// set the model's vertex color
void Components::Sprite3D::setColor(const float(&color)[4])
{
  model->setColor(color);
}

// get the Sprite3D's model
Models::Model* Components::Sprite3D::getModel()
{
  return model;
}

// set alpha
void Components::Sprite3D::setAlpha(float newAlpha)
{
  alpha = newAlpha;
}

// get alpha
float Components::Sprite3D::getAlpha()
{
  return alpha;
}
