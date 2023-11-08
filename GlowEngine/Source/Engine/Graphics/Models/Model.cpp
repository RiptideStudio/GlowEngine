/*
/
// filename: Model.cpp
// author: Callen Betts
// brief: implements Model.h
/
*/

#include "stdafx.h"
#include "Model.h"
#include "Engine/GlowEngine.h"
#include "Engine/Graphics/Renderer.h"
#include "Engine/Systems/Parsing/ObjectLoader.h"
#include "Engine/Graphics/Models/ModelLibrary.h"

// construct a new base model
Models::Model::Model()
  :
  modelType(ModelType::Cube),
  indexBuffer(nullptr),
  vertexBuffer(nullptr)
{
  init();
}

// construct a model given a filename
Models::Model::Model(const std::string fileName)
  :
  modelType(ModelType::Cube),
  indexBuffer(nullptr),
  vertexBuffer(nullptr)
{
  init();
  load(fileName);
}

// initialize the model
void Models::Model::init()
{
  engine = EngineInstance::getEngine();
  renderer = engine->getRenderer(); // models need the renderer to draw
  device = renderer->getDevice();
  deviceContext = renderer->getDeviceContext();
  dirty = true;
}

// parse a .obj file and load its vertex data into the model
// adds the model to the model library if it doesn't already exist
void Models::Model::load(const std::string fileName)
{

  // query the model library to see if we can get a clone
  Models::ModelLibrary* library = engine->getModelLibrary();
  Models::Model* model = library->get(fileName);

  if (model)
  {
    // if the model is in the library
    indices = model->getIndices();
    vertices = model->getVerticies();
  }
  else // if it is not in the library
  {
    // parse a 3D model
    Parse::ObjectLoader modelData;
    modelData.open(fileName);
    modelData.parse();

    // store the indices and vertices
    indices = modelData.getVertexIndices();
    vertices = modelData.getVertices();
  }

  // update the buffers for index and vertex data
  // this will also create them if they don't exist
  updateVertexBuffer();
  updateIndexBuffer();
}

// get vertices contianer
const std::vector<GlowMath::Vertex>& Models::Model::getVerticies()
{
  return vertices;
}

// get indices container
const std::vector<unsigned short>& Models::Model::getIndices()
{
  return indices;
}

// creates the vertex buffer or updates it 
void Models::Model::updateVertexBuffer()
{
  D3D11_BUFFER_DESC vertexBufferDesc = {};
  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertices.size();
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;

  D3D11_SUBRESOURCE_DATA vertexData = {};
  vertexData.pSysMem = vertices.data();
  renderer->getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
}

// creates a model's index buffer or updates it 
void Models::Model::updateIndexBuffer()
{
  D3D11_BUFFER_DESC indexBufferDesc = {};
  indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  indexBufferDesc.ByteWidth = sizeof(unsigned short) * indices.size();
  indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.CPUAccessFlags = 0;

  D3D11_SUBRESOURCE_DATA indexData = {};
  indexData.pSysMem = indices.data();
  renderer->getDevice()->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
}

// get the index buffer for read
ID3D11Buffer* Models::Model::getIndexBuffer()
{
  return indexBuffer;
}

// set the texture coordinates to scale with a vector for tiling
void Models::Model::setUV(Vector3D coords)
{
  for (auto& v : vertices)
  {
    v.tx *= coords.x;
    v.ty *= coords.y;
  }
  updateVertexBuffer();
  dirty = false;
}

// set a model's color
void Models::Model::setColor(const float (&col)[4])
{
  // loop through the vertices and set color
  for (auto& vertex : vertices)
  {
    vertex.r = col[0];
    vertex.g = col[1];
    vertex.b = col[2];
    vertex.a = col[3];
  }
  // update the vertex buffer
  updateVertexBuffer();
}

// get the vertex buffer for read
ID3D11Buffer* Models::Model::getVertexBuffer()
{
  return vertexBuffer;
}

// render a model
void Models::Model::render()
{
  // bind the vertex buffer and index buffer
  deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
  deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);

  // draw the triangle
  deviceContext->DrawIndexed(indices.size(), 0, 0);
}
