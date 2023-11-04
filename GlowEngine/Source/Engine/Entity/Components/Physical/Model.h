/*
/
// filename: Model.h
// author: Callen Betts
// brief: defines the model class, holds vertices and model data
/
*/

#pragma once
#include "Engine/Entity/Components/Component.h"
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")


// forward declare renderer
namespace Graphics
{
  class Renderer;
}

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

    // create the vertex buffer for a model
    void updateVertexBuffer();
    // create the index buffer for a model
    void updateIndexBuffer();
    // get the buffers for read only
    ID3D11Buffer* getVertexBuffer();
    ID3D11Buffer* getIndexBuffer();

    // render a model
    void render();

  protected:
    
    // give the models access to the renderer/engine
    Engine::GlowEngine* engine;
    Graphics::Renderer* renderer;

    // all inherited objects have these properties
    ModelType modelType;
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;

    // define the vertex stride and offset
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    // define the vertices and indices containers
    std::vector<GlowMath::Vertex> vertices;
    std::vector<unsigned short> indices;

  };

}