/*
/
// filename: Widget.cpp
// author: Callen Betts
// brief: implements Widget.h
/
*/

#include "stdafx.h"
#include "Widget.h"

Editor::Widget::Widget(std::string title_, std::string desc, ImGuiWindowFlags flags_)
  :
  title(title_),
  description(desc),
  flags(flags_)
{

}

// main update frame, calls render of all elements
void Editor::Widget::renderFrame()
{
  if (!loaded)
  {
    init();
    loaded = true;
  }

  beginFrame();
  update();
  endFrame();
}

// called on all widgets
void Editor::Widget::beginFrame()
{
  ImGui::Begin(title.c_str(), nullptr, flags);
}

void Editor::Widget::endFrame()
{
  ImGui::Text(description.c_str());
  ImGui::End();
}
