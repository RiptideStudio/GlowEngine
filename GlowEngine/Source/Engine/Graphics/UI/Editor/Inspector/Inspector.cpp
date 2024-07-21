/*
/
// filename: Inspector.cpp
// author: Callen Betts
// brief: implements Inspector.h
/
*/

#include "stdafx.h"
#include "Inspector.h"

Editor::Inspector::Inspector(std::string title, std::string desc, ImGuiWindowFlags flags): Widget(title, desc, flags)
{

}

void Editor::Inspector::update()
{
	// if we have a selected entity, we allow changing its properties
	if (selectedEntity)
	{
		Components::Transform* transform = getComponentOfType(Transform, selectedEntity);
		transform->recalculateMatrix();

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();

		// for each variable type, allow modification; we can only edit variable types explicitly defined
		for (auto& variable : selectedEntity->getVariables())
		{
			variable.display();
		}

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();

		// iterate over every component's properties; each serializable property will be modifiable
		// this is done using a vector of properties in each component
		for (const auto& component : selectedEntity->getComponents())
		{
			std::string componentName = component->getName();

			if (ImGui::TreeNode(componentName.c_str())) // begin component tree node *
			{
				for (auto& variable : component->getVariables())
				{
					ImGui::NewLine();

					// label the variable
					variable.display();
				}

				ImGui::NewLine();
				ImGui::TreePop(); // end component tree node *
			}
		}

	}
}
