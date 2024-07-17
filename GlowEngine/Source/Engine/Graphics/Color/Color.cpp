/*
/
// filename: Color.cpp
// author: Callen Betts
// brief: implements Color.h
/
*/

#include "stdafx.h"
#include "Color.h"

namespace Graphics
{
  const Color Color::Red = Color(1, 0, 0, 1);
  const Color Color::Green = Color(0, 1, 0, 1);
  const Color Color::Blue = Color(0, 0, 1, 1);
  const Color Color::White = Color(1, 1, 1, 1);
  const Color Color::Clear = Color(0,0,0,0);
}