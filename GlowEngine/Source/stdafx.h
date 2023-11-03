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

// directX math
#include <DirectXMath.h>

// standard includes
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>

// json & deserialization
#include "Engine/Systems/Parsing/json.hpp"

// engine
#include "Engine/EngineInstance.h"

// math
#include "Engine/Math/GlowMath.h"
#include "Engine/Math/Vertex.h"

using namespace GlowMath;

// rendering
#include "Windows/Resource.h"

// systems
#include "Engine/Systems/Logger/Log.h"

// components
#include "Engine/Entity/Components/Physical/Model.h"
#include "Engine/Entity/Components/Physical/Transform.h"
#include "Engine/Entity/Components/Component.h"