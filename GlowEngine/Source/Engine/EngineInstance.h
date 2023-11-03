/*
/
// filename: EngineInstance.h
// author: Callen Betts
// brief: defines an instance of the GlowEngine
//
// description: the members here are static so we can access them from anywhere
/
*/

#pragma once

namespace Engine
{
  class GlowEngine;
}

class EngineInstance
{

  public:

    static Engine::GlowEngine* getEngine();
    static void setup(Engine::GlowEngine* engine);

  private:

    static Engine::GlowEngine* engine;

}; // EngineInstance