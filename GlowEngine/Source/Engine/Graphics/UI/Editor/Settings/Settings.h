/*
/
// filename: Settings.h
// author: Callen Betts
// brief: defines a settings window for the Editor
/
*/

#pragma once
#include "Engine/Graphics/UI/Editor/Widget.h"
#include "GameSettings.h"

namespace Editor
{

	class Settings : public Widget
	{

	public:

		Settings(std::string title, std::string desc = "", ImGuiWindowFlags flags = 0) : Widget(title, desc, flags) {};

		void update();

	private:

		// define setting subsections here; separated into classes for containerization
		GameSettings gameSettings;

	};

}