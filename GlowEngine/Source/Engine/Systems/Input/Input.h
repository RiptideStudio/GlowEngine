/*
/
// filename: Input.h
// author: Callen Betts
// brief: defines input system for handling basic input with mouse and keyboard
/
*/

#pragma once
#include "Game/System/System.h"

namespace Input
{


  class InputSystem : public Systems::System
  {

  public:

    InputSystem(std::string systemName);

    // general game loop update
    void update();
    // update key states
    void updateKeyStates();
    // update hotkeys
    void updateHotkeys();

    // get the mouse delta
    Vector3D getMouseDelta() { return { (float)mouseDelta.x, (float)mouseDelta.y,0 }; }

    // called when windows triggered a key
    void onKeyTriggered(int keycode);

    // called when windows released a key
    void onKeyRelease(int keycode);

    // if a key is being held down
    bool keyDown(int key);

    // if a key was triggered
    bool keyTriggered(int key);

    // if a key was released
    bool keyReleased(int key);

    // if we are focused on this window
    bool isFocused() { return focused; }
    // set focused
    void setFocus(bool focus);

  private:

    // mouse data
    int scroll;
    POINT mouseDelta;
    POINT currentMousePosition;
    POINT previousMousePosition;

    bool focused;

    HWND windowHandle;

    // keystates
    std::unordered_map<int, bool> keystates;
    std::unordered_map<int, bool> previousKeystates;

  };

}