/*
/
// filename: Input.cpp
// author: Callen Betts
// brief: implements the input system
/
*/

#include "stdafx.h"
#include "Input.h"

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
