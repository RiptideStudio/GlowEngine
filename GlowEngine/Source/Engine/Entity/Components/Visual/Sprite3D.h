/*
/
// filename: Sprite3D.h
// author: Callen Betts
// brief: defines 3D sprite class for use with models
/
*/

#pragma once

namespace Graphics
{
  class Renderer;
}

namespace Textures
{
  class Texture;
}

namespace Components
{

  class Sprite3D : public Component
  {

  public:

    // constructors
    Sprite3D(const std::string modelName, const std::string textureName = "");
    Sprite3D();

    Sprite3D* clone();
    virtual void load(const nlohmann::json&);

    // initialize
    void init();
    // render - this calls model->render() as well
    void render();

    // set the sprite's model
    void setModel(const std::string modelName);
    // model helper functions
    void setColor(const float(&color)[4]);
    // set a sprite's texture map to the model's texture names
    void setTextures(std::string singleTextureName);
    // set a texuture to repeat
    void setTextureRepeat(bool val);

    // ImGui
    void inspect();

    // get the model
    Models::Model* getModel();
    
    // set alpha
    void setAlpha(float alpha);
    // get alpha
    float getAlpha();

    // single texture mode
    void setSingleTextureMode(bool val) { singleTexture = val; }
    bool isSingleTextureMode() { return singleTexture; }

  private:

    float alpha;

    bool repeatTexture = false;
    bool singleTexture = true;

    Models::Model* model;
    std::map<std::string, Textures::Texture*> textures; // map of textures to objects
    Textures::Texture* texture;
    Graphics::Renderer* renderer;

  };

}