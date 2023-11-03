/*
/
// filename: Component.h
// author: Callen Betts
// brief: defines Component class
/
*/

#pragma once

namespace Components
{

  class Component
  {

    public:

      virtual void init() {};
      virtual void update() {};
      virtual void exit() {};


    protected:
      // all components have access to this
      
      
    private:

  };

}