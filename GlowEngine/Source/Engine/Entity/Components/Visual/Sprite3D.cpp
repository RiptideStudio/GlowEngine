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
#include "Engine/EngineInstance.h"

// overloaded constructor to take in a model and a texture
Components::Sprite3D::Sprite3D(const std::string modelName, const std::string textureName)
  : Component(),
  renderer(nullptr)
{
  init();
  setTextures(textureName);
}

// base Sprite3D constructor to give pointers to renderer
Components::Sprite3D::Sprite3D()
  : Component(),
  renderer(nullptr)
{
  init();
}

Components::Sprite3D* Components::Sprite3D::clone()
{
  return new Sprite3D(*this);
}

void Components::Sprite3D::load(const nlohmann::json& data) 
{
  if (data.contains("singleTexture")) setSingleTextureMode(data["singleTexture"]);
  if (data.contains("model")) setModel(data["model"]);
  if (data.contains("texture")) setTextures(data["texture"]); else setTextures("None");
}

// initialize the Sprite3D component
void Components::Sprite3D::init()
{
  model = new Models::Model();
  texture = nullptr;
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
    if (model->isDirty() && repeatTexture)
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
  renderer->updateObjectBufferWorldMatrix(transform->getTransformMatrix());

  // bind the constant buffer and update subresource
  renderer->updateObjectBuffer();

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
  model->load(modelName);
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

  // single texture mode is auto-set if objects are only one
  if (singleTexture)
  {
    // any objects that have less than one model should just set all textures to the given one
    for (int i = 0; i < objects; ++i)
    {
      textures[model->getModelNames()[i]] = new Textures::Texture(singleTextureName);
    }
  }
  else
  {
    for (int i = 0; i < objects; ++i)
    {
      // BUG: This current implementation implies that the number of textures matches the number of objects
      // we need to make it so that is not the case
      std::string modelName = model->getModelNames()[i];

      // never over-index the textures
      int textureIndex = i;
      if (textureIndex >= model->getTextureModelNames().size())
      {
        textureIndex = model->getTextureModelNames().size() - 1;
      }
      std::string textureName = model->getTextureModelNames()[textureIndex];
      textures[modelName] = texLib->get(textureName);
    }
  }

}

void Components::Sprite3D::setTextureRepeat(bool val)
{
  repeatTexture = val;
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
