/*
/
// filename: Animation3D.cpp
// author: Callen Betts
// brief: implements Animation3D.h
/
*/

#include "stdafx.h"
#include "Animation3D.h"

// initialize the animation's base values
Components::Animation3D::Animation3D()
  :
  frame(0),
  totalFrames(1),
  frameSpeed(0.1f),
  frameCounter(0),
  running(false),
  loop(true)
{
  type = ComponentType::Animation3D;
}

// update the logic of the animation and change the frame
void Components::Animation3D::update()
{
  // don't update if not running
  if (!running)
  {
    return;
  }

  // function callback for start frame
  if (frame == 0 && frameCounter == 0)
  {
    start();
  }

  // increment the frame
  frameCounter += frameSpeed;
  if (frameCounter >= 1)
  {
    frameCounter = 0;
    frame++;
  }

  // end the animation, invoke end function callback
  if (frame >= totalFrames)
  {
    end();
    stop(); // function callback
  }
}

// play an animation
void Components::Animation3D::play()
{
  running = true;
}

// play an advanced animation
void Components::Animation3D::play(int startFrame, float animationSpeed, bool isLooping)
{
  running = true;
  loop = isLooping;
  frameSpeed = animationSpeed;
  frame = startFrame;
}

// reset an animation's base values - used for stopping or testing animations
void Components::Animation3D::end()
{
  frame = 0;
  frameCounter = 0;
  if (!loop)
  {
    running = false;
  }
}
