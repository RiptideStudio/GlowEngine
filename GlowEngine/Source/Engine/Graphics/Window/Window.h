/*
/
// filename: Window.h
// author: Callen Betts
// brief: defines Window class
/
*/

#pragma once

namespace Graphics
{

  class Window
  {

  public:

    Window();

    // setup the window
    bool setup();

    // update
    void updateWindow();

    // windows callbacks
    LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK windowProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    // message updates
    bool updateMessages();

    HWND getHandle() { return windowHandle; }
    int getWidth() { return windowWidth; }
    int getHeight() { return windowHeight; }

    const std::wstring getName() { return windowName; }
    void setName(std::wstring name) { windowName = name; }

    // get the message parameter from input
    int getMessageParam();

  private:

    // window properties
    std::wstring windowName;
    std::wstring windowClassName;
    HWND windowHandle;
    HINSTANCE hInstance;

    int windowWidth;
    int windowHeight;

    // windows messages
    MSG msg = {};

    // pointers
    Input::InputSystem* input;
    Engine::GlowEngine* engine;
  };

}