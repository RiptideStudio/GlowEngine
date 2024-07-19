/*
/
// filename: Widget.h
// author: Callen Betts
// brief: defines Widget class
/
*/

#pragma once

namespace UI
{

  class Widget
  {

  public:

    Widget(std::string name, std::string desc = "", ImGuiWindowFlags flags_ = ImGuiWindowFlags_NoCollapse);

    virtual void render() {};

    void renderFrame();
    void beginFrame();
    void endFrame();

  protected:

    std::string title;
    std::string description;

    ImGuiWindowFlags flags;
  };
}