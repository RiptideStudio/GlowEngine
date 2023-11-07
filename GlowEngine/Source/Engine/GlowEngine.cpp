/*
/
// filename: GlowEngine.cpp
// author: Callen Betts
// brief: implements GlowEngine.h
/
*/

#include "stdafx.h"
#include "GlowEngine.h"
#include "Game/Scene/Scene.h"
#include "Engine/Graphics/Renderer.h"
#include "Engine/Graphics/Models/ModelLibrary.h"
#include "Engine/Graphics/Textures/TextureLibrary.h"
#include "Game/Scene/SceneSystem.h"

// define the systems vector here
// this way we can access it from the base system class
std::vector<Systems::System*> SystemInstance::systems;

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
  EngineInstance::setup(this);
  windowClassName = L"Otherglow Window";
  windowName = L"Otherglow";
  fps = 60;
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

  // create early systems
  createCoreSystems();
  // create later systems
  createLaterSystems();

  // success
  Logger::write("Done!");
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
  float frameCount = 0;
  float fpsTimer = 0;

  // windows messages
  MSG msg = {};
  LARGE_INTEGER frequency, prevTime, currTime;

  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&prevTime);

  // run the engine
  running = true;

  while (running)
  {
    // terminate engine on escape
    if (input->keyDown(VK_ESCAPE))
    {
      stop();
    }
    // windows messages
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      if (msg.message == WM_QUIT)
      {
        break;
      }
    }

    // calculate the delta time
    QueryPerformanceCounter(&currTime);
    deltaTime = static_cast<float>(currTime.QuadPart - prevTime.QuadPart) / frequency.QuadPart;
    prevTime = currTime;
    fpsTimer += deltaTime;
    frameCount++;

    // update systems
    update();
    // render systems
    render();

    if (fpsTimer >= 1.0f)
    {
      int fps = frameCount;
      frameCount = 0;
      fpsTimer -= 1.0f;

      Logger::write("Real FPS: "+std::to_string(fps));
    }
  } 

  // stop running
  stop();
  return static_cast<int>(msg.wParam);
}

// stop all processes in the engine and exit
void Engine::GlowEngine::stop()
{
  Logger::write("Stopping engine");
  running = false;
}

// loop through and update each system
void Engine::GlowEngine::update()
{
  // update input 
  input->update();

  // update all systems
  for (auto system : SystemInstance::getSystems())
  {
    system->update();
  }
}

// call the renderer updates and render systems
void Engine::GlowEngine::render()
{
  // start render frame update
  renderer->beginFrame();

  // render all systems
  sceneSystem->render();

  // end renderer frame and present screen
  renderer->endFrame();
}

// exit the engine and cleanup
void Engine::GlowEngine::exit()
{
  cleanUp();
}

// create systems that need to be initialized first
void Engine::GlowEngine::createCoreSystems()
{
  // input
  input = new Input::InputSystem();
  // setup renderer
  renderer = new Graphics::Renderer(windowHandle);
}

// create systems dependent on core systems
void Engine::GlowEngine::createLaterSystems()
{
  // model library
  modelLibrary = new Models::ModelLibrary();
  modelLibrary->load();
  // texture library
  textureLibrary = new Textures::TextureLibrary();
  textureLibrary->load();
  // scene system
  sceneSystem = new Scene::SceneSystem("SceneSystem");
}

// setup the window
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
  }
}

// called on engine exit
void Engine::GlowEngine::cleanUp()
{
  Logger::write("Cleaning up...");
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

// add a system to the system list
void SystemInstance::addSystem(Systems::System* system)
{
  systems.push_back(system);
}
