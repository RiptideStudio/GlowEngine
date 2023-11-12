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
#include "Engine/Graphics/Window/Window.h"
#include "Engine/Graphics/Meshes/MeshLibrary.h"

// initialize engine values
Engine::GlowEngine::GlowEngine()
  :
  running(false),
  deltaTime(0),
  frameTime(0),
  renderer(nullptr),
  input(nullptr),
  modelLibrary(nullptr),
  sceneSystem(nullptr),
  textureLibrary(nullptr),
  window(nullptr),
  windowHandle(NULL),
  totalFrames(0)
{
  EngineInstance::setup(this);
  fps = 60;
}

// start the engine - returns false if failed, true on success
bool Engine::GlowEngine::start()
{
  // we need a window immediately
  window = new Graphics::Window();

  // create the window
  Logger::write("Starting engine...");

  // failed
  if (!window->setup())
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
  window->updateWindow();

  // calculate delta time
  float dt = 0.f;
  float totalTime = 0.f;
  float frameCount = 0;
  float fpsTimer = 0;

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
    if (!window->updateMessages())
    {
      break;
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
  return window->getMessageParam();
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
  // window handle
  windowHandle = getWindowHandle();
  // input
  input = new Input::InputSystem("InputSystem");
  // setup renderer
  renderer = new Graphics::Renderer(windowHandle);
}

// create systems dependent on core systems
void Engine::GlowEngine::createLaterSystems()
{
  // mesh library
  meshLibrary = new Meshes::MeshLibrary();
  meshLibrary->load();
  // model library
  modelLibrary = new Models::ModelLibrary();
  modelLibrary->load();
  // texture library
  textureLibrary = new Textures::TextureLibrary();
  textureLibrary->load();
  // scene system
  sceneSystem = new Scene::SceneSystem("SceneSystem");
  sceneSystem->init();
}

// called on engine exit
void Engine::GlowEngine::cleanUp()
{
  Logger::write("Cleaning up...");
}

// directly get the window handle
HWND Engine::GlowEngine::getWindowHandle()
{
  return window->getHandle();
}
