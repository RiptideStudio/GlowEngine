/*
/
// filename: Component.cpp
// author: Callen Betts
// brief: implements Component.h
/
*/

#include "stdafx.h"
#include "Property.h"

Variable CreateVariable(std::string name, int* val)
{
  return Variable(name, val);
}

Variable CreateVariable(std::string name, float* val)
{
  return Variable(name, val);
}

Variable CreateVariable(std::string name, Vector3D* val)
{
  return Variable(name, val);
}

Variable CreateVariable(std::string name, bool* val)
{
  return Variable(name, val);
}

Variable CreateVariable(std::string name, std::string* val)
{
  return Variable(name, val);
}