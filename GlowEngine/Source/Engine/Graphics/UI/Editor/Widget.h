/*
/
// filename: Widget.h
// author: Callen Betts
// brief: defines Widget class
/
*/

#pragma once

namespace Graphics
{
  class GlowGui;
}

namespace Editor
{

  class Widget
  {

  public:

    Widget(std::string name, std::string desc = "", ImGuiWindowFlags flags_ = ImGuiWindowFlags_NoCollapse);

    virtual void update() {};
    virtual void init() {};

    void renderFrame();
    void beginFrame();
    void endFrame();

    Graphics::GlowGui* ui;

  protected:

    std::string title;
    std::string description;

    ImGuiWindowFlags flags;

    bool loaded = false;
  };
}