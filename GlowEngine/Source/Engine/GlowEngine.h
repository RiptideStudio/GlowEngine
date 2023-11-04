/*
/
// filename: GlowEngine.h
// author: Callen Betts
// brief: defines GlowEngine class
/
*/

#pragma once

// forward declare renderer
namespace Graphics
{
  class Renderer;
}

namespace Engine
{
  
  class GlowEngine
  {

  public:
    GlowEngine();

    // handle engine states
    bool start();
    bool run();
    void stop();

    void update();
    void render();
    void exit();

    // setup window
    void setupWindow();

    // windows callbacks
    LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK windowProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    // cleanup on engine exit
    void cleanUp();

    // get system pointers
    Input::InputSystem* getInputSystem();
    Graphics::Renderer* getRenderer();

    // engine properties
    int getFps();
    int getTotalFrames();
    bool isRunning();
    float getDeltaTime();
    HWND getWindowHandle();

  private:

    // window properties
    std::wstring windowName;
    std::wstring windowClassName;
    HWND windowHandle;
    HINSTANCE hInstance;

    int windowWidth;
    int windowHeight;

    // system pointers
    Graphics::Renderer* renderer;
    Input::InputSystem* input;

    // core engine properties (fps, delta time)
    bool running;
    int fps;
    int totalFrames;
    int frameTime;
    float deltaTime;

  };

}