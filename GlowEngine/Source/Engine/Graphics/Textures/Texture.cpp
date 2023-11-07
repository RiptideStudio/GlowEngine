/*
/
// filename: Texture.cpp
// author: Callen Betts
// brief: implements Texture.h
/
*/

#include "stdafx.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"
#include "Engine/GlowEngine.h"
#include "Engine/EngineInstance.h"
#include "Engine/Graphics/Renderer.h"
#include "Engine/Graphics/Textures/TextureLibrary.h"

// default constructor
Textures::Texture::Texture()
{
  init();
}

// overloaded constructor to load an image immediately
Textures::Texture::Texture(std::string name)
{
  init();
  load(name);
}

// initialize texture properties
void Textures::Texture::init()
{
  // give textures a pointer to the texture library
  Engine::GlowEngine* engine = EngineInstance::getEngine();
  textureLibrary = engine->getTextureLibrary();
  renderer = engine->getRenderer();
  texture2D = nullptr;
  textureView = nullptr;
  textureDesc = {};
}

// load a texture given a filename 
// this will search the texture library and fetch the resource, or create it if it does not exist
void Textures::Texture::load(std::string fileName)
{
  // check the texture library to see if we have a texture
  Textures::Texture* texture = textureLibrary->get(fileName);
  if (texture)
  {
    // found texture, set pointer
    *this = *texture;
    return;
  }
  else
  {
    // could not find texture
    data = stbi_load(fileName.c_str(), &width, &height, &channels, 4);
    if (!data)
    {
      // if data was invalid
      Logger::error("Failed to load texture " + fileName);
    }
  }

  // set properties
  name = fileName;

  // create/update the texture resource
  createTextureResource();
}

void Textures::Texture::createTextureResource()
{
  // setup the texture description and resource
  textureDesc.Width = width;
  textureDesc.Height = height;
  textureDesc.MipLevels = 1;
  textureDesc.ArraySize = 1;
  textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // this must match the stbi_load format
  textureDesc.SampleDesc.Count = 1;
  textureDesc.Usage = D3D11_USAGE_DEFAULT;
  textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  textureDesc.CPUAccessFlags = 0;

  // set the system memory
  subResource.pSysMem = data;
  subResource.SysMemPitch = width * 4;
  subResource.SysMemSlicePitch = 0;

  // create the texture
  renderer->getDevice()->CreateTexture2D(&textureDesc, &subResource, &texture2D);
  if (texture2D)
  {
    renderer->getDevice()->CreateShaderResourceView(texture2D, nullptr, &textureView);
  }
}
