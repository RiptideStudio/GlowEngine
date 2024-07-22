/*
/
// filename: GameWindow.cpp
// author: Callen Betts
// brief: implements GameWindow.h
/
*/

#include "stdafx.h"
#include "GameWindow.h"
#include "Engine/Graphics/Window/Window.h"
#include "Engine/GlowEngine.h"
#include "Engine/Graphics/Renderer.h"
#include "Engine/Graphics/UI/Editor/Inspector/Inspector.h"
#include "Engine/Graphics/Camera/Camera.h"
#include "Game/Scene/Scene.h"
#include "Game/Scene/SceneSystem.h"

// when we are focused on our game window, we can click on objects to select them
void Editor::GameWindow::update()
{
  // render the game to an ImGui texture
  ID3D11ShaderResourceView* texture = EngineInstance::getEngine()->getRenderer()->GetGameTexture();
  if (texture)
  {
    ImGui::Image((void*)texture, { (float)Graphics::Window::GetWidth(),(float)Graphics::Window::GetHeight() });
  }

  // Check if the game window is focused and a mouse click occurred
  if (ImGui::IsWindowFocused() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) 
  {
    ImVec2 mousePos = ImGui::GetMousePos();
    Vector3D screenCoords(mousePos.x, mousePos.y, 0.f);

    Visual::Camera* camera = EngineInstance::getEngine()->getCamera();

    // Get the view and projection matrices
    DirectX::XMMATRIX viewMatrix = camera->getViewMatrix();
    DirectX::XMMATRIX perspectiveMatrix = camera->getPerspecitveMatrix();

    // Convert screen coordinates to world coordinates
    Vector3D worldCoords = Vector3D::ScreenToWorldCoords(screenCoords);

    // Perform ray picking
    Entities::Entity* selectedEntity = RayPickEntity(worldCoords, viewMatrix, perspectiveMatrix);
    Inspector::inspect(selectedEntity);
  }
}

// grab an entity by casting a ray into the scene based on our mouse coordinates, and then return that entity
Entities::Entity* Editor::GameWindow::RayPickEntity(Vector3D worldCoords, DirectX::XMMATRIX view, DirectX::XMMATRIX perspective)
{
  Visual::Camera* camera = EngineInstance::getEngine()->getCamera();

  Vector3D rayOrigin = Vector3D::XMVectorToVector3D(camera->getPosition());
  Vector3D rayDirection = worldCoords - rayOrigin; // Compute the direction of the ray

  rayDirection.normalize();

  // Perform ray picking in the scene
  Entities::Entity* pickedEntity = EngineInstance::getEngine()->getSceneSystem()->getCurrentScene()->RayPick(rayOrigin, rayDirection);
  return pickedEntity;
}

// calculate the size of our game window
void Editor::GameWindow::calculateGameWindowSize()
{
  // calculate the aspect ratio
  const float aspectRatio = EngineInstance::getEngine()->getWindow()->getAspectRatio();

  availableSize = ImGui::GetContentRegionAvail();

  // Calculate the new size to maintain the aspect ratio
  float newWidth = availableSize.x;
  float newHeight = availableSize.x / aspectRatio;

  if (newHeight > availableSize.y)
  {
    newHeight = availableSize.y;
    newWidth = availableSize.y * aspectRatio;
  }

  // Set the new size of the ImGui window based on the aspect ratio
  ImVec2 newSize(newWidth, newHeight);
  ImGui::SetNextWindowSize(newSize);

  // Update the size after setting the new window size
  availableSize = newSize;
}
