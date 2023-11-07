/*
/
// filename: System.h
// author: Callen Betts
// brief: defines the base system class
/
*/

#pragma once

namespace Systems
{

  class System
  {

  public:

    System(std::string systemName);

    // each system is initialized, updated, or rendered
    virtual void init() {};
    virtual void update() {};
    virtual void render() {};

    // get the system's name
    const std::string getName() { return name; }

  protected:

    // all systems have access to core engine and other important systems
    Engine::GlowEngine* engine;

    std::string name;
  };

}