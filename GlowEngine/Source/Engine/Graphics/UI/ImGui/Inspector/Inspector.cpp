/*
/
// filename: Inspector.cpp
// author: Callen Betts
// brief: implements Inspector.h
/
*/

#include "stdafx.h"
#include "Inspector.h"

UI::Inspector::Inspector(std::string title, std::string desc, ImGuiWindowFlags flags): Widget(title, desc, flags)
{
}

void UI::Inspector::update()
{
	// if we have a selected entity, we allow changing its properties
	if (selectedEntity)
	{
		// name field
		std::string entityName = selectedEntity->getName();

		char buffer[256];
		strncpy_s(buffer, entityName.c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = 0;

		if (ImGui::InputText("Name", buffer, sizeof(buffer)))
		{
			selectedEntity->setName(std::string(buffer));
		}
		for (const auto& component : selectedEntity->getComponents())
		{
			std::string componentName = component->getName();
			if (ImGui::TreeNode(componentName.c_str())) // begin component tree node *
			{

				component->inspect();

				ImGui::TreePop(); // end component tree node *
			}
		}
	}
}
