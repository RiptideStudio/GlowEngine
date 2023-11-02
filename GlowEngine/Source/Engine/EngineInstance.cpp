/*
/
// filename: EngineInstance.cpp
// author: Callen Betts
// brief: implements EngineInstance.h
/
*/

#include "stdafx.h"
#include "EngineInstance.h"

Engine::GlowEngine* EngineInstance::engine = nullptr;

// this lets us get the active instance of the engine
Engine::GlowEngine* EngineInstance::getEngine()
{
  return engine;
}
