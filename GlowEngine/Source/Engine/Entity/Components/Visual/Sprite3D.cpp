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
#include "Engine/Graphics/Lighting/Shadows/ShadowSystem.h"
#include "Engine/Entity/Components/Visual/Models/ModelLibrary.h"

REGISTER_COMPONENT(Sprite3D);

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

Components::Sprite3D::Sprite3D(const Sprite3D& other) : Component(other)
{
  init();
  setModel(other.model->getName());
  textures = other.textures;
}

Components::Sprite3D* Components::Sprite3D::clone()
{
  return new Sprite3D(*this);
}

void Components::Sprite3D::load(const nlohmann::json& data) 
{
}

/// <summary>
/// Refreshes the model after being loaded
/// </summary>
void Components::Sprite3D::CustomLoad(const nlohmann::json saveData)
{
  if (model)
  {
    if (saveData.contains("Model")) setModel(saveData["Model"]["value"]);
    
    // Add textures
    if (saveData.contains("Texture"))
    {
      std::string texName = saveData["Texture"]["value"];

      if (texName != "")
      {
        setTextures(texName);
      }
    }
  }
}

/// <summary>
/// Save things like textures and meshes
/// </summary>
/// <param name=""> The json object </param>
void Components::Sprite3D::CustomSave(nlohmann::json saveData) const
{

}

// initialize the Sprite3D component
void Components::Sprite3D::init()
{
  model = new Models::Model("Cube");
  texture = nullptr;
  type = Components::Component::Sprite3D;
  name = "Sprite3D";
  Engine::GlowEngine* engine = EngineInstance::getEngine();
  renderer = engine->getRenderer();

  AddVariable(CreateVariable("Repeat Texture", &repeatTexture));
  AddVariable(CreateVariable("Shadow", &drawShadow));
  AddVariable(CreateVariable("Model", &(model->getName())));
  AddVariable(CreateVariable("Texture", &(textureName)));
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

  // check if this sprite has a texture
  if (!textures.empty())
  {
    // change the UVs
    if (repeatTexture)
    {
      renderer->SetUVScale(transform->getScale().x, transform->getScale().y);
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

    DrawOutline();
  }

  // reset UV scale after drawing
  renderer->SetUVScale(1, 1);

 /* if (drawShadow)
    renderer->GetShadowSystem()->DrawShadow(transform->getPosition(), transform->getScale());*/
}

// draw the outline of this sprite 
void Components::Sprite3D::DrawOutline()
{
  if (parent->IsSelected())
  {
    Components::Transform* transform = getComponentOfType(Transform, parent);
    renderer->DrawSetOutline(Color::Outline);
    Vector3D outlineScale = transform->getScale() * 1.02f;
    Vector3D oldScale = transform->getScale();
    transform->setScale(outlineScale);
    transform->recalculateMatrix();
    renderer->updateObjectBufferWorldMatrix(transform->getTransformMatrix());
    renderer->updateObjectBuffer();
    transform->setScale(oldScale);
    model->render();
    renderer->DrawSetOutline(Color::Clear);
  }
}

void Components::Sprite3D::display()
{
  Models::ModelLibrary* lib = EngineInstance::getEngine()->getModelLibrary();
  Textures::TextureLibrary* tex = EngineInstance::getEngine()->getTextureLibrary();

  // Assuming `currentModel` is a member variable that holds the name of the current model
  static std::string currentModel = "";  // You might want to initialize this with your default model

  if (ImGui::TreeNode("Models"))
  {
    for (const auto& modelEntry : lib->models)
    {
      const std::string& modelName = modelEntry.first;
      bool isSelected = (modelName == currentModel);

      if (ImGui::Selectable(modelName.c_str(), isSelected))
      {
        // Update the current model when the selectable item is clicked
        currentModel = modelName;

        // Update your model here, e.g., loading the new model into your Sprite3D component
        setModel(modelName); // Assuming setModel is a method to update the model
      }

      // Keep the selection highlighted if it matches the current model
      if (isSelected)
      {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::TreePop();
  }

  if (ImGui::TreeNode("Textures"))
  {
    for (const auto& entry : tex->textures)
    {
      const std::string& name = entry.first;
      bool isSelected = (name == currentModel);

      if (ImGui::Selectable(name.c_str(), isSelected))
      {
        currentModel = name;
        setTextures(name);
      }

      if (isSelected)
      {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::TreePop();
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
