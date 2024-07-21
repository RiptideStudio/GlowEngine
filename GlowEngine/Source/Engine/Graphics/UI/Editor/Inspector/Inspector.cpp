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
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();

		// for each variable type, allow modification; we can only edit variable types explicitly defined
		for (auto& variable : selectedEntity->getVariables())
		{
			ImGui::Text(variable.name.c_str());

			switch (variable.type)
			{
			case VariableType::String:
				ImGui::InputText(("##" + variable.name).c_str(), std::get<std::string*>(variable.value)->data(), 256);
				break;
			case VariableType::Bool:
				ImGui::Checkbox(("##" + variable.name).c_str(), std::get<bool*>(variable.value));
				break;
			case VariableType::Int:
				ImGui::InputInt(("##" + variable.name).c_str(), std::get<int*>(variable.value));
				break;
			case VariableType::Float:
				ImGui::InputFloat(("##" + variable.name).c_str(), std::get<float*>(variable.value));
				break;
			case VariableType::Vector:
				Vector3D* vec = std::get<Vector3D*>(variable.value);
				ImGui::InputFloat(("##" + variable.name + "x").c_str(), &vec->x);
				ImGui::InputFloat(("##" + variable.name + "y").c_str(), &vec->y);
				ImGui::InputFloat(("##" + variable.name + "z").c_str(), &vec->z);
				break;
			}
			ImGui::NewLine();
		}
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
					ImGui::Text(variable.name.c_str());

					// for each variable type, allow modification; we can only edit variable types explicitly defined
					switch (variable.type)
					{
					case VariableType::Int:
						ImGui::InputInt(("##" + variable.name).c_str(), std::get<int*>(variable.value));
						break;
					case VariableType::Float:
						ImGui::InputFloat(("##" + variable.name).c_str(), std::get<float*>(variable.value));
						break;
					case VariableType::Bool:
						ImGui::Checkbox(("##" + variable.name).c_str(), std::get<bool*>(variable.value));
						break;
					case VariableType::String:
						ImGui::InputText(("##" + variable.name).c_str(), std::get<std::string*>(variable.value)->data(), 256);
						break;
					case VariableType::Vector:
						Vector3D* vec = std::get<Vector3D*>(variable.value);
						ImGui::InputFloat(("##" + variable.name + "x").c_str(), &vec->x);
						ImGui::InputFloat(("##" + variable.name + "y").c_str(), &vec->y);
						ImGui::InputFloat(("##" + variable.name + "z").c_str(), &vec->z);
						break;
					}
				}

				ImGui::NewLine();
				ImGui::TreePop(); // end component tree node *
			}
		}
	}
}
