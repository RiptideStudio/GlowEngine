/*
/
// filename: System.cpp
// author: Callen Betts
// brief: implements System.h
/
*/

#include "stdafx.h"
#include "System.h"
#include "Engine/GlowEngine.h"

// create a system that requires a name
// systems are automatically added to the active systems so long as they call the base constructor
Systems::System::System(std::string systemName)
{
  engine = EngineInstance::getEngine();
  input = engine->getInputSystem();
  name = systemName;
  SystemInstance::addSystem(this);
}
