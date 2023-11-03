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
    // called when windows triggered a key
    void keyTriggered(int keycode);

    // called when windows released a key
    void keyReleased(int keycode);

    // if a key is being held down
    bool keyDown(int key);


  private:
    // keystates
    std::unordered_map<int, bool> keystates;

  };

}