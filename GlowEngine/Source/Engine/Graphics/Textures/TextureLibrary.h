/*
/
// filename: TextureLibrary.h
// author: Callen Betts
// brief: defines TextureLibrary class
/
*/

#pragma once
#include "Engine/Graphics/Textures/Texture.h"

namespace Textures
{

  class TextureLibrary
  {

  public:

    TextureLibrary();

    void init();

  private:
    std::map<std::string, Textures::TextureLibrary> textures;

  };

}