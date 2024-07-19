/*
/
// filename: GlowGui.cpp
// author: Callen Betts
// brief: implements GlowGui.h
/
*/

#include "stdafx.h"
#include "GlowGui.h"
#include "Engine/Graphics/Renderer.h"
#include "Engine/Graphics/Window/Window.h"

Graphics::GlowGui::GlowGui(HWND windowHandle, ID3D11Device* device, ID3D11DeviceContext* context, Graphics::Renderer* renderer)
  :
  windowHandle(windowHandle),
  device(device),
  context(context),
  renderer(renderer)
{
  // initialize the ImGui system
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplWin32_Init(windowHandle);
  ImGui_ImplDX11_Init(device, context);
  ImGui::StyleColorsDark();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Graphics::GlowGui::beginUpdate()
{
  // initialize a new ImGui frame
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();

  // Create a docking space
  ImGuiIO& io = ImGui::GetIO();
  ImVec2 size = { (float)renderer->getWindow()->getWidth(), (float)renderer->getWindow()->getHeight() };

  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
  {

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
      ImGuiWindowFlags_NoBackground;

    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));
    ImGui::End();
  }
}

// anything called between the begin and end update is rendered
// here, we will invoke any draw calls
void Graphics::GlowGui::update()
{
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
    ImGuiWindowFlags_NoScrollWithMouse;

  // Begin the ImGui window with the specified flags
  ImGui::Begin("Game View", nullptr, window_flags);
  ImVec2 size = ImGui::GetContentRegionAvail();

  // Adjust the viewport to match the ImGui window
  D3D11_VIEWPORT viewport = {};
  viewport.TopLeftX = ImGui::GetWindowPos().x;
  viewport.TopLeftY = ImGui::GetWindowPos().y+20;
  viewport.Width = size.x+16;
  viewport.Height = size.y+16;
  viewport.MinDepth = 0.0f;
  viewport.MaxDepth = 1.0f;
  renderer->getDeviceContext()->RSSetViewports(1, &viewport);
  ImGui::End();

  // Additional UI windows
  ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoCollapse);
  ImGui::Text("This is the Inspector window.");
  ImGui::End();

  // Create another additional dockable window
  ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoCollapse);
  ImGui::Text("This is the Hierarchy window.");
  ImGui::End();

  // Create another additional dockable window
  ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoCollapse);
  ImGui::Text("This is the Hierarchy window.");
  ImGui::End();

  // Create another additional dockable window
  ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoCollapse);
  ImGui::Text("This is the Console window.");
  ImGui::End();

  // Create another additional dockable window
  ImGui::Begin("Resources", nullptr, ImGuiWindowFlags_NoCollapse);
  ImGui::Text("This is the Console window.");
  ImGui::End();

  endUpdate();
}

void Graphics::GlowGui::endUpdate()
{
  // end the frame and display it
  ImGui::EndFrame();
  ImGui::Render();
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

// clean up ImGui resources
void Graphics::GlowGui::cleanUp()
{
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
}


