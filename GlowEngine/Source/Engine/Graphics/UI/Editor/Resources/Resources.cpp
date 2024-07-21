/*
/
// filename: stub.cpp
// author: Callen Betts
// brief: implements stub.h
/
*/

#include "stdafx.h"
#include "Resources.h"
#include "Engine/GlowEngine.h"
#include "Engine/Graphics/Textures/TextureLibrary.h"
#include "Engine/Graphics/Renderer.h"

void Editor::Resources::update()
{
	Graphics::Renderer* renderer = EngineInstance::getEngine()->getRenderer();

	// TODO: checkbox for enabling debug mode
}
