/*
/
// filename: GlowEngine.h
// author: Callen Betts
// brief: defines GlowEngine class
/
*/

#pragma once

// forward declare systems
namespace Graphics { class Renderer; }
namespace Models { class ModelLibrary; }
namespace Textures { class TextureLibrary; }
namespace Scene { class SceneSystem; }

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

    // create core systems
    void createCoreSystems();
    // create systems dependent on core systems
    void createLaterSystems();

    // setup window
    void setupWindow();

    // windows callbacks
    LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK windowProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    // cleanup on engine exit
    void cleanUp();

    // get system pointers
    Input::InputSystem* getInputSystem() { return input; }
    Graphics::Renderer* getRenderer() { return renderer; }
    Models::ModelLibrary* getModelLibrary() { return modelLibrary; }
    Textures::TextureLibrary* getTextureLibrary() { return textureLibrary; }
    Scene::SceneSystem* getSceneSystem() { return sceneSystem; }

    // get engine properties
    int getFps() { return fps; }
    int getTotalFrames() { return totalFrames; }
    bool isRunning() { return running; }
    float getDeltaTime() { return deltaTime; }
    HWND getWindowHandle() { return windowHandle; }

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
    Models::ModelLibrary* modelLibrary;
    Textures::TextureLibrary* textureLibrary;
    Scene::SceneSystem* sceneSystem;

    // core engine properties (fps, delta time)
    bool running;
    int fps;
    int totalFrames;
    int frameTime;
    float deltaTime;

  };
}

// define an instance of the system list so we can access and create systems in
// the base class
class SystemInstance
{

public:

  // add a system to the vector - system base class constructor does this automatically
  static void addSystem(Systems::System* system);
  // get the systems vector
  static std::vector<Systems::System*> getSystems() { return systems; }

private:

  // engine contains a vector of systems
  static std::vector<Systems::System*> systems;

};
