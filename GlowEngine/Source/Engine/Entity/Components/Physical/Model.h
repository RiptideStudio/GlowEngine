/*
/
// filename: Model.h
// author: Callen Betts
// brief: defines the model class, holds vertices and model data
/
*/

#pragma once
#include "Engine/Entity/Components/Component.h"

namespace Components
{

  class Model : public Component
  {

    // enum for primitive 3D models (cubes, spheres, etc.)
    enum ModelType
    {
      Cube,
      Sphere,
      Pyramid
    };

  public:
    Model();

    // loading a model will either load the vertices based on its ModelType or load a .obj file
    // if no .obj file is found, it will default to the ModelType
    void load(const std::string fileName);
    // load an object by its primitive ModelType
    void loadPrimitive();
    // get vertices and indices container
    const std::vector<GlowMath::Vertex>& getVerties();
    const std::vector<unsigned short>& getIndices();

  protected:
    ModelType modelType;

  private:
    // define the vertices and indices containers
    std::vector<GlowMath::Vertex> vertices;
    std::vector<unsigned short> indices;

  };

}