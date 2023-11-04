/*
/
// filename: GlowEngine.cpp
// author: Callen Betts
// brief: implements GlowEngine.h
/
*/

#include "stdafx.h"
#include "GlowEngine.h"
#include "Engine/Graphics/Renderer.h"

// initialize engine values
Engine::GlowEngine::GlowEngine()
  :
  windowWidth(1280),
  windowHeight(720),
  running(false),
  deltaTime(0),
  frameTime(0),
  renderer(nullptr)
{
  windowClassName = L"Otherglow Window";
  windowName = L"Otherglow";
  fps = 60;
  // create systems
  input = new Input::InputSystem();
}

// start the engine - returns false if failed, true on success
bool Engine::GlowEngine::start()
{
  // create the window
  Logger::write("Starting engine...");
  setupWindow();

  // failed
  if (!windowHandle)
  {
    Logger::error("Invalid window handle");
    return false;
  }

  // success
  Logger::write("Success");
  return true;
}

// run the engine
bool Engine::GlowEngine::run()
{
  // update and display the window
  UpdateWindow(windowHandle);
  ShowWindow(windowHandle, true);

  // calculate delta time
  float dt = 0.f;
  float totalTime = 0.f;

  // windows messages
  MSG msg = {};
  LARGE_INTEGER frequency, prevTime, currTime;

  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&prevTime);

  // run the engine
  running = true;

  while (running)
  {
    // Handle window messages
    // use GetMessage() if you want to stop updates when not focused in window
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      if (msg.message == WM_QUIT)
      {
        break;
      }
    }

    // calculate delta time and frames finished
    QueryPerformanceCounter(&currTime);
    dt = static_cast<float>(currTime.QuadPart - prevTime.QuadPart) / frequency.QuadPart;
    prevTime = currTime;
    totalTime += dt;
    deltaTime = totalTime;

    // Check if it's time to update the screen + input
    if (totalTime >= frameTime)
    {
      // set the delta time
      deltaTime = totalTime;     
      // update all systems
      update();
      // render all systems
      render();
      // finish game frame and calculate delta time
      totalFrames++;
      totalTime -= frameTime;
    }
    totalTime = 0;
  }

  // stop running
  running = false;
  return static_cast<int>(msg.wParam);
}

// stop all processes in the engine and exit
void Engine::GlowEngine::stop()
{
  Logger::write("Stopping engine");
  running = false;
}

// update all systems
void Engine::GlowEngine::update()
{

}

// call the renderer updates and render systems
void Engine::GlowEngine::render()
{
  renderer->beginFrame();
  renderer->testUpdate();
  renderer->endFrame();
}

// exit the engine and cleanup
void Engine::GlowEngine::exit()
{
  cleanUp();
}

void Engine::GlowEngine::setupWindow()
{

  // register the window class
  hInstance = GetModuleHandle(nullptr);

  WNDCLASSEX wc = {};
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = windowProcStatic;
  wc.hInstance = hInstance;
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.lpszClassName = windowClassName.c_str();

  RegisterClassEx(&wc);

  // Create window handle
  windowHandle = CreateWindowEx(
    0, // window style
    windowClassName.c_str(), // class name
    windowName.c_str(), // window title
    WS_OVERLAPPEDWINDOW, // window style
    CW_USEDEFAULT, // initial horizontal position of the window
    CW_USEDEFAULT, // initial vertical position of the window
    windowWidth, // width
    windowHeight, // height
    nullptr, // window parent (none)
    nullptr, // window child (none)
    hInstance, // window instance handle
    this // Set the GWLP_USERDATA to the GlowEngine instance
  );

  // position the window to be in the centre of the screen
  if (windowHandle != nullptr)
  {
    // Adjust the window size
    RECT rect = { 0, 0, static_cast<LONG>(windowWidth), static_cast<LONG>(windowHeight) };
    AdjustWindowRect(&rect, GetWindowLong(windowHandle, GWL_STYLE), FALSE);
    SetWindowPos(windowHandle, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
    // setup renderer
    renderer = new Graphics::Renderer(windowHandle);
  }

}

// called on engine exit
void Engine::GlowEngine::cleanUp()
{
  Logger::write("Cleaning up...");
}

// get the input system
Input::InputSystem* Engine::GlowEngine::getInputSystem()
{
  return input;
}

// get the renderer
Graphics::Renderer* Engine::GlowEngine::getRenderer()
{
  return renderer;
}

// get the engine's FPS
int Engine::GlowEngine::getFps()
{
  return fps;
}

// return the total number frames passed in game
int Engine::GlowEngine::getTotalFrames()
{
  return totalFrames;
}

// get whether or not the engine is running
bool Engine::GlowEngine::isRunning()
{
  return running;
}

// get delta time
float Engine::GlowEngine::getDeltaTime()
{
  return deltaTime;
}

// get the window handle
HWND Engine::GlowEngine::getWindowHandle()
{
  return windowHandle;
}

// windows message callback
LRESULT Engine::GlowEngine::windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
    // when a key is first triggered, set the keystate to active
  case WM_KEYDOWN:
    input->onKeyTriggered(wParam);
    break;

    // when a key is released, you should reset the keystate and call "released"
  case WM_KEYUP:
    input->onKeyRelease(wParam);
    break;

    // destroy the window and stop the engine
  case WM_DESTROY:
    PostQuitMessage(0);
    stop();
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

// windows callback function
LRESULT Engine::GlowEngine::windowProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  // Retrieve the pointer to the LynxEngine instance
  GlowEngine* enginePtr = reinterpret_cast<GlowEngine*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

  if (message == WM_CREATE)
  {
    // Set the GWLP_USERDATA value to the LynxEngine instance when the window is created
    CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
    enginePtr = reinterpret_cast<GlowEngine*>(createStruct->lpCreateParams);
    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(enginePtr));
  }
  
  return enginePtr->windowProc(hWnd, message, wParam, lParam);
}
