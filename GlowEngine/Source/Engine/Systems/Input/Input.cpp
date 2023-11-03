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
void Input::InputSystem::keyTriggered(int keycode)
{
  keystates[keycode] = true;
}

// reset a keystate
void Input::InputSystem::keyReleased(int keycode)
{
  keystates[keycode] = false;
}

// return whether or not a key is being held down
bool Input::InputSystem::keyDown(int key)
{
  return keystates[key];
}
