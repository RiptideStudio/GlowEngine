/*
/
// filename: Collider.cpp
// author: Callen Betts
// brief: implements Collider.h
/
*/

#include "stdafx.h"
#include "Collider.h"
#include "Engine/GlowEngine.h"
#include "Engine/Graphics/Renderer.h"

void Components::Collider::render()
{
  if (debugDraw)
  {
    Graphics::Renderer* renderer = EngineInstance::getEngine()->getRenderer();

    renderer->setRasterizerFillMode(D3D11_FILL_WIREFRAME);
    renderDebug();
    renderer->setRasterizerFillMode(D3D11_FILL_SOLID);
  }
}
