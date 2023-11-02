/*
/
// filename: stdafx.h
// author: Callen Betts
// brief: precompiled header
/
*/

#pragma once

// windows
#include "Windows/targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

// engine
#include "Engine/EngineInstance.h"
#include "Engine/GlowEngine.h"

// rendering
#include "Windows/Resource.h"
#include "Engine//Graphics/Renderer.h"

// directX
#include <DirectXMath.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

// standard includes
#include <string>
#include <map>
#include <vector>

