/*
/
// filename: SceneEditor.cpp
// author: Callen Betts
// brief: implements SceneEditor.h
/
*/

#include "stdafx.h"
#include "SceneEditor.h"
#include "Engine/Entity/Entity.h"
#include "Engine/Entity/EntityList/EntityList.h"
#include "Engine/GlowEngine.h"
#include "Game/Scene/SceneSystem.h"
#include "Engine/Graphics/UI/Editor/Inspector/Inspector.h"

Editor::SceneEditor::SceneEditor(std::string title, std::string desc, ImGuiWindowFlags flags) : Widget(title, desc, flags)
{
	currentScene = nullptr;
	sceneSystem = nullptr;
	selectedContainer = nullptr;
	selectedEntity = nullptr;
}

void Editor::SceneEditor::init()
{
	sceneSystem = EngineInstance::getEngine()->getSceneSystem();
	currentScene = sceneSystem->getCurrentScene();
}

void Editor::SceneEditor::update()
{
	// add containers, move them around
	interact();

	// Check for right-click within the scene for new container
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("NewContainer");
	}

	// for each list of entities, we want to display them and their name
	int i = 0;
	for (const auto& wrapper : currentScene->getEntityWrappers())
	{
		const auto& container = wrapper->list;

		// select this container so we can add to it!
		ImVec2 start = ImGui::GetCursorScreenPos();
		ImVec2 end = ImVec2(start.x + ImGui::GetContentRegionAvail().x, start.y + ImGui::GetTextLineHeightWithSpacing());

		// Check for right-click within the bounding box
		if (ImGui::IsMouseHoveringRect(start, end) && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("NewObject");
		}

		if (ImGui::TreeNode(wrapper->name.c_str())) // start container menu *
		{
			// if collapsed, don't display entities
			// display each entity in the container
			DragContainer(wrapper,i);

			int j = 0;
			for (const auto& entity : container->getEntities())
			{
				// click on the entity to inspect it 
				if (ImGui::Selectable(entity->getName().c_str())) // start entity menu *
				{
					selectedEntity = entity;
					Inspector::inspect(entity); // set the inspector
				}

				// drag a selectable button (this lets us reorganize our tree)
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
				{
					// Set the payload to carry the entity pointer
					ImGui::SetDragDropPayload("ENTITY", &j, sizeof(int));
					ImGui::Text(entity->getName().c_str());

					// end the drag sourrce
					ImGui::EndDragDropSource();
				}

				// Set the drop target to reposition the entity
				if (ImGui::BeginDragDropTarget()) 
				{
					// set the entity's order within the list
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY")) 
					{
						int srcIndex = *(const int*)payload->Data;
						int dstIndex = j;
						container->ReorderEntities(srcIndex, dstIndex);
					}
					ImGui::EndDragDropTarget();
				}

				j++; // end of entity menu *
			}

			ImGui::TreePop(); // end container menu *
		}
		else
		{
			DragContainer(wrapper,i);
		}
		i++;
	}
}

// used to add new entity containers to the scene hierarchy when we right click
void Editor::SceneEditor::interact()
{
	// create a new object
	if (ImGui::BeginPopup("NewObject"))
	{
		if (ImGui::MenuItem("Add New Object"))
		{
			if (selectedContainer)
			{
				Entities::Actor* ent = new Entities::Actor();
				currentScene->addToList(selectedContainer->list, ent);
			}
		}
		ImGui::EndPopup();
	}

	// create a new container list to the hierarchy
	if (ImGui::BeginPopup("NewContainer"))
	{
		if (ImGui::MenuItem("Add New Container"))
		{
			currentScene->addEntityList("Container");
		}
		ImGui::EndPopup();
	}
}

void Editor::SceneEditor::DragContainer(Entities::EntityListWrapper* wrapper, int i)
{
	// drag a selectable button(this lets us reorganize our tree)
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
	{
		// Set the payload to carry the entity pointer
		ImGui::SetDragDropPayload("ENTITYLIST", &i, sizeof(int));
		ImGui::Text(wrapper->name.c_str());

		// end the drag sourrce
		ImGui::EndDragDropSource();
	}

	// Set the drop target to reposition the entity
	if (ImGui::BeginDragDropTarget())
	{
		// set the entity's order within the list
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITYLIST"))
		{
			int srcIndex = *(const int*)payload->Data;
			int dstIndex = i;
			currentScene->ReorderLists(srcIndex, dstIndex);
		}
		ImGui::EndDragDropTarget();
	}
}
