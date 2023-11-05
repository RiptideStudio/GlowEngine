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
    Sprite3D(const std::string modelName);
    Sprite3D();

    // initialize
    void init();
    // render - this calls model->render() as well
    void render();

    // set the sprite's model
    void setModel(const std::string modelName);
    // model helper functions
    void setColor(const float(&color)[4]);

    // get the model
    Models::Model* getModel();
    
    // set alpha
    void setAlpha(float alpha);
    // get alpha
    float getAlpha();

  private:

    float alpha;

    Models::Model* model;
    Textures::Texture* texture;
    Graphics::Renderer* renderer;

  };

}