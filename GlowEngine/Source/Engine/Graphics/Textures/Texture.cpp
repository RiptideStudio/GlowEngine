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
  textureLibrary = nullptr;
  texture2D = nullptr;
  textureView = nullptr;
  textureDesc = {};
  load("Assets/StoneBrick.png");
}

// load a texture given a filename 
// this will search the texture library and fetch the resource, or create it if it does not exist
void Textures::Texture::load(std::string fileName)
{
  stbi_set_flip_vertically_on_load(true);
  // load the file
  data = stbi_load(fileName.c_str(), &width, &height, &channels, 0);
  if (!data) 
  {
    // if data was invalid
    Logger::error("Failed to load texture "+fileName);
  }

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

  // create the texture and shader resource view
  EngineInstance::getEngine()->getRenderer()->getDevice()->CreateTexture2D(&textureDesc, &subResource, &texture2D);
  EngineInstance::getEngine()->getRenderer()->getDevice()->CreateShaderResourceView(texture2D, nullptr, &textureView);
}
