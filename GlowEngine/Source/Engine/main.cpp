/*
/
// filename: main.cpp
// author: Callen Betts
// brief: implements main game loop
/
*/

#include "stdafx.h"
#include "Engine/GlowEngine.h"

// create engine
static Engine::GlowEngine* engine = new Engine::GlowEngine();

int main()
{
  // setup the engine
  EngineInstance::setup(engine);

  // start the engine
  if (engine->start())
  {
    engine->run();
  }

  // terminate program
  Logger::write("Terminated program");
  return -1;
}