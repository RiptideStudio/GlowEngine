/*
/
// filename: Texture.h
// author: Callen Betts
// brief: defines Texture class
/
*/

#pragma once


namespace Textures
{

  class TextureLibrary; // forward declare

  class Texture
  {

  public:
    Texture();

  private:
    Textures::TextureLibrary* textureLibrary;
    std::string name;

  };

}