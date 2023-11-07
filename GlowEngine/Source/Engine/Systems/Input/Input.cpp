/*
/
// filename: Input.cpp
// author: Callen Betts
// brief: implements the input system
/
*/

#include "stdafx.h"
#include "Input.h"
#include "Engine/GlowEngine.h"
#include "Engine/EngineInstance.h"

Input::InputSystem::InputSystem()
{
  windowHandle = EngineInstance::getEngine()->getWindowHandle();
  previousMousePosition = { 0 };
  currentMousePosition = { 0 };
  mouseDelta = { 0 };
  scroll = 0;
  focused = true;
}

// update the input state
void Input::InputSystem::update()
{
  // calculate mouse delta
  if (GetCursorPos(&currentMousePosition))
  {
    // Calculate mouse delta
    mouseDelta.x = currentMousePosition.x - previousMousePosition.x;
    mouseDelta.y = currentMousePosition.y - previousMousePosition.y;
  }

  // toggle the focus
  if (keyDown('F'))
  {
    focused = true;
  }
  // toggle the focus
  if (keyDown('P'))
  {
    focused = false;
  }
  if (focused)
  {
    ShowCursor(false);
    // focus the mouse in the center of the screen
    RECT clientRect;
    GetClientRect(windowHandle, &clientRect);
    POINT center = { (clientRect.right - clientRect.left) / 2, (clientRect.bottom - clientRect.top) / 2 };
    ClientToScreen(windowHandle, &center);
    SetCursorPos(center.x, center.y);

    // update previous mouse position to the center
    previousMousePosition.x = center.x;
    previousMousePosition.y = center.y;
  }
  else
  {
    ShowCursor(true);
  }
}

// set a key state to active
void Input::InputSystem::onKeyTriggered(int keycode)
{
  // key is held down
  keystates[keycode] = true;
}

// reset a keystate
void Input::InputSystem::onKeyRelease(int keycode)
{
  keystates[keycode] = false;
}

// return whether or not a key is being held down
bool Input::InputSystem::keyDown(int key)
{
  return keystates[key];
}

// check if a key was triggered
bool Input::InputSystem::keyTriggered(int key)
{
  // TO BE IMPLEMENTED
  return false;
}
