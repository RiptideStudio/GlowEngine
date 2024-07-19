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
#include "Engine/Graphics/Renderer.h"

Input::InputSystem::InputSystem(std::string systemName) 
  : System(systemName)
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
  if (focused)
  {
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
  }
}

void Input::InputSystem::updateKeyStates()
{
  updateHotkeys();
  previousKeystates = keystates;
}

void Input::InputSystem::updateHotkeys()
{
  // toggle fullscreen
  if (keyReleased(VK_TAB))
  {
    engine->getRenderer()->toggleFullscreen();
  }
  // terminate engine on escape
  if (keyTriggered(VK_ESCAPE))
  {
    if (!focused)
    {
      engine->stop();
    }
    else
    {
      focused = false;
    }
  }
  // toggle renderer debug mode
  if (keyTriggered('Q'))
  {
    engine->getRenderer()->toggleDebugMode();
  }

  // toggle the focus
  if (keyTriggered('P'))
  {
    focused = !focused;
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
  return keystates[key] && !previousKeystates[key];
}

bool Input::InputSystem::keyReleased(int key)
{
  return previousKeystates[key] && !keystates[key];
}

void Input::InputSystem::setFocus(bool focus)
{
  focused = focus;
}
