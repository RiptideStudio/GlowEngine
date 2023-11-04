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
#include "Engine/Entity/Entity.h"
#include <filesystem>

// initialize a new model
Components::Model::Model()
  :
  modelType(ModelType::Cube),
  indexBuffer(nullptr),
  vertexBuffer(nullptr)
{
  type = ComponentType::model;
  engine = EngineInstance::getEngine();
  renderer = engine->getRenderer(); // models contain our vertex data and buffers
  device = renderer->getDevice();
  deviceContext = renderer->getDeviceContext();
  load("Data/Models/Cube/Cube.json");
  // create the buffers for index and vertex data
  updateVertexBuffer();
  updateIndexBuffer();
}

// parse a .obj file and load its vertex data into the model
// will instead load primitive type if object file was invalid
void Components::Model::load(const std::string fileName)
{
  std::ifstream file(fileName);
  nlohmann::json data;
  file >> data;

  if (file.is_open())
  {
    // load the vertices
    for (auto vertex : data["vertices"])
    {
      Vertex v;

      v.x = vertex["v"][0];
      v.y = vertex["v"][1];
      v.z = vertex["v"][2];
      vertices.push_back(v);
    }
    // load the indices
    for (auto index : data["indices"])
    {
      indices.push_back(index);
    }
  }
}

// load a model based on its model type
void Components::Model::loadPrimitive()
{

}

// get vertices contianer
const std::vector<GlowMath::Vertex>& Components::Model::getVerties()
{
  return vertices;
}

// get indices container
const std::vector<unsigned short>& Components::Model::getIndices()
{
  return indices;
}

// creates the vertex buffer or updates it 
void Components::Model::updateVertexBuffer()
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
void Components::Model::updateIndexBuffer()
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
ID3D11Buffer* Components::Model::getIndexBuffer()
{
  return indexBuffer;
}

// get the vertex buffer for read
ID3D11Buffer* Components::Model::getVertexBuffer()
{
  return vertexBuffer;
}

// render a model
void Components::Model::render()
{
  // load transform data
  Components::Transform* transform = getParent()->getComponent<Components::Transform>(ComponentType::transform);
  if (!transform)
  {
    return;
  }
  
  // update the constant buffer's world matrix
  renderer->updateConstantBufferWorldMatrix(transform->getTransformMatrix());

  // update the constant buffer
  renderer->updateConstantBuffer();

  // bind the vertex buffer and index buffer
  deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
  deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);

  // draw the triangle
  deviceContext->DrawIndexed(indices.size(), 0, 0);
}
