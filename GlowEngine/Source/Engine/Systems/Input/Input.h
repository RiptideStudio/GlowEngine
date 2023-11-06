/*
/
// filename: Input.h
// author: Callen Betts
// brief: defines input system for handling basic input with mouse and keyboard
/
*/

#pragma once

namespace Input
{


  class InputSystem
  {

  public:

    InputSystem();

    // update the keystates and mouse state
    void update();

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

  private:

    // mouse data
    int scroll;
    POINT mouseDelta;
    POINT currentMousePosition;
    POINT previousMousePosition;

    HWND windowHandle;

    // keystates
    std::unordered_map<int, bool> keystates;
    std::unordered_map<int, bool> keystatesTriggered;

  };

}