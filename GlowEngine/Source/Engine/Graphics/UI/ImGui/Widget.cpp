/*
/
// filename: Widget.cpp
// author: Callen Betts
// brief: implements Widget.h
/
*/

#include "stdafx.h"
#include "Widget.h"

UI::Widget::Widget(std::string title_, std::string desc, ImGuiWindowFlags flags_)
  :
  title(title_),
  description(desc),
  flags(flags_)
{

}

// main update frame, calls render of all elements
void UI::Widget::renderFrame()
{
  beginFrame();
  render();
  endFrame();
}

// called on all widgets
void UI::Widget::beginFrame()
{
  ImGui::Begin(title.c_str(), nullptr, flags);
}

void UI::Widget::endFrame()
{
  ImGui::Text(description.c_str());
  ImGui::End();
}
