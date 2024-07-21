/*
/
// filename: Model.h
// author: Callen Betts
// brief: defines the model class, holds vertices and model data
/
*/

/* 

HOW IT WORKS:

// we have a map of the different vertices and indices for each object
// this is because a model might be exported with several objects "attached"
// for example, if we have two cubes in blender and we want to export them, it will only render the original cube
//  or the first cube it read. By having a map of the different objects that were exported, we can individually save
//  and keep track of each one's vertices and indices. The map will store the object's name (for example, "cube1" and "cube2")
//  and each of that object's respective data.
//
// modelVertices refers to the actual map of all of the object's vertices 
// modelIndices refers to the map of all the object's indices, rather than individual objects

*/

#pragma once
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")


// forward declare renderer
namespace Graphics
{
  class Renderer;
}

namespace Models
{

  class Model
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
    Model(const std::string fileName);

    void init();

    // loading a model will either load the vertices based on its ModelType or load a .obj file
    // if no .obj file is found, it will default to the ModelType
    void load(const std::string fileName);
    // get vertices and indices container
    const std::vector<GlowMath::Vertex>& getVerticies();
    const std::vector<GlowMath::Vertex>& getVerticies() const;
    const std::vector<unsigned short>& getIndices();

    // get the model vertex map
    const std::map<std::string, std::vector<Vertex>>& getModelVertices() { return modelVertices; }
    const std::map<std::string, std::vector<Vertex>>& getModelVertices() const { return modelVertices; }
    // model indices
    const std::map<std::string, std::vector<unsigned short>>& getModelIndices() { return modelIndices; }
    // get the model names - this is so we can iterate over each name and get them from the map
    const std::vector<std::string>& getModelNames() { return modelNames; }
    // get the texture names
    std::vector<std::string> getTextureModelNames() { return textureNames; }
    // get the number of objects
    const int& getObjects() { return objects; }
    // set the name and get the name
    const void setName(std::string newName) { name = newName; }
    const std::string getName() const { return name; }
    std::string& getName() { return name; }

    // get the model's buffer index
    ID3D11Buffer* getVertexBuffer(int index);
    ID3D11Buffer* getIndexBuffer(int index);

    // render a circular shadow
    void renderShadow();

    // create the vertex buffer for a model
    void updateVertexBuffer();
    // create the index buffer for a model
    void updateIndexBuffer();

    // set the object index to render
    void setObjectIndex(int index) { objectIndex = index; }
    // if we are single texture

    // uv coordinates
    void setUV(Vector3D coords);
    // model helper functions
    void setColor(const float(&color)[4]);

    // is dirty
    bool isDirty() { return dirty; }

    // render a model
    void render();

  private:
    
    bool dirty; // recalculate vertices, indices, or texture coords

    // give the models access to the renderer/engine
    Engine::GlowEngine* engine;
    Graphics::Renderer* renderer;

    // all inherited objects have these properties
    ModelType modelType;
    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;

    // we need our models to have maps of buffers which contain the vertex and index data
    std::map<std::string, ID3D11Buffer*> vertexBuffers;
    std::map<std::string, ID3D11Buffer*> indexBuffers;

    // models have a shadow buffer
    ID3D11Buffer* shadowBuffer;

    // define the vertex stride and offset
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    // define the vertices and indices containers
    // we have a map of the different vertices and indices for each object
    std::map<std::string, std::vector<Vertex>> modelVertices;
    std::map<std::string, std::vector<unsigned short>> modelIndices;
    std::vector<std::string> modelNames;
    std::vector<std::string> textureNames;

    int objects; // how many objects are in the model
    int objectIndex; // which index of the object we should render

    // will be deprecated soon
    std::vector<Vertex> vertices;
    std::vector<unsigned short> indices;

    std::string name;

  };

}