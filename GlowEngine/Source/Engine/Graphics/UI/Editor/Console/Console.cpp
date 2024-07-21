/*
/
// filename: stub.cpp
// author: Callen Betts
// brief: implements stub.h
/
*/

#include "stdafx.h"
#include "Console.h"
#include "Engine/GlowEngine.h"
#include "Engine/Graphics/Textures/TextureLibrary.h"

void Editor::Console::update()
{
  // Create another additional dockable window
  ID3D11ShaderResourceView** r = EngineInstance::getEngine()->getTextureLibrary()->get("PlayButton")->getTextureView();

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); // Transparent button background
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0)); // No padding

  if (ImGui::ImageButton((void*)(*r), { 64,64 }))
  {
    EngineInstance::getEngine()->getInputSystem()->setFocus(!EngineInstance::getEngine()->getInputSystem()->isFocused());
  }

  ImGui::PopStyleVar();
  ImGui::PopStyleColor(1);
}
