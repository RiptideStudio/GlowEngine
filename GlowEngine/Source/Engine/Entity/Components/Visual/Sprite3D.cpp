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
#include "Engine/Graphics/Textures/Texture.h"

// overloaded constructor to take in a model and a texture
Components::Sprite3D::Sprite3D(const std::string modelName, const std::string textureName)
  : Component(),
  renderer(nullptr),
  model(nullptr)
{
  init();
  setModel(modelName);
  setTextures();
}

// base Sprite3D constructor to give pointers to renderer
Components::Sprite3D::Sprite3D()
  : Component(),
  renderer(nullptr),
  model(nullptr),
  texture(nullptr)
{
  init();
}

// initialize the Sprite3D component
void Components::Sprite3D::init()
{
  type = ComponentType::sprite3D;
  name = "Sprite-3D";
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

  // check if this sprite has a texture
  if (!textures.empty())
  {
    // change the UVs
    if (model->isDirty())
    {
      model->setUV(transform->getScale());
    }
  }

  // update the constant buffer's world matrix
  renderer->updateConstantBufferWorldMatrix(transform->getTransformMatrix());

  // bind the constant buffer and update subresource
  renderer->updateConstantBuffer();

  // loop through textures given model names
  for (auto name : model->getModelNames())
  {
    if (textures[name])
    {
      renderer->getDeviceContext()->PSSetShaderResources(0, 1, textures[name]->getTextureView());
    }
    else
    {
      // unbind the shader resource and reset texture
      renderer->unBindTexture();
    }


    // render the model
    model->render(name);
  }
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

// set the textures this sprite uses based on the model
void Components::Sprite3D::setTextures()
{
  if (model->getTextureModelNames().size() > 0)
  {
    for (int i = 0; i < model->getObjects(); ++i)
    {
      textures[model->getModelNames()[i]] = new Textures::Texture(model->getTextureModelNames()[i]);
    }
  }
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
