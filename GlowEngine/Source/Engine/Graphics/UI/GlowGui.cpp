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
}

void Graphics::GlowGui::update()
{
  // anything called between the begin and end update is rendered
  // here, we will invoke any draw calls
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


