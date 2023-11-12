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
#include "Engine/Graphics/Textures/TextureLibrary.h"


// overloaded constructor to take in a model and a texture
Components::Sprite3D::Sprite3D(const std::string modelName, const std::string textureName)
  : Component(),
  renderer(nullptr),
  model(nullptr)
{
  init();
  setModel(modelName);
  setTextures(textureName);
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
  type = ComponentType::Sprite3D;
  name = "Sprite-3D";
  Engine::GlowEngine* engine = EngineInstance::getEngine();
  renderer = engine->getRenderer();
}

// render a Sprite3D's model
void Components::Sprite3D::render()
{
  // set transform constant buffer
  Components::Transform* transform = getComponentOfType(Transform, parent);
  if (!transform)
  {
    return;
  }
  else
  {
    if (transform->isDirty())
    {
      transform->recalculateMatrix();
    }
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
  else
  {
    // unbind the shader resource and reset texture
    renderer->unBindTexture();
  }

  // update the constant buffer's world matrix
  renderer->updateConstantBufferWorldMatrix(transform->getTransformMatrix());

  // bind the constant buffer and update subresource
  renderer->updateConstantBuffer();

  // each models contain several objects, we need to loop through them 
  for (int i = 0; i < model->getObjects(); ++i)
  {
    // set the texture - each object in a model might have a different texture
    std::string name = model->getModelNames()[i];

    if (textures[name])
    {
      renderer->getDeviceContext()->PSSetShaderResources(0, 1, textures[name]->getTextureView());
    }

    // set the object index to be rendered
    model->setObjectIndex(i);

    // render the model
    model->render();
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
void Components::Sprite3D::setTextures(std::string singleTextureName)
{
  Textures::TextureLibrary* texLib = EngineInstance::getEngine()->getTextureLibrary();

  // go through each model and set the textures
  int objects = model->getObjects();
  int textureNum = model->getTextureModelNames().size(); // how many individual textures we have

  if (textureNum > 1)
  {
    for (int i = 0; i < objects; ++i)
    {
      std::string modelName = model->getModelNames()[i];
      std::string textureName = model->getTextureModelNames()[i];
      textures[modelName] = texLib->get(textureName);
    }
  }
  else
  {
    // any objects that have less than one model should just set all textures to the given one
    for (int i = 0; i < objects; ++i)
    {
      textures[model->getModelNames()[i]] = new Textures::Texture(singleTextureName);
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
