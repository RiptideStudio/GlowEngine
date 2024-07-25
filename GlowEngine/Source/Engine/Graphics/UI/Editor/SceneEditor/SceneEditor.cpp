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
#include "Engine/Graphics/Camera/Camera.h"
#include "Engine/Graphics/Textures/TextureLibrary.h"

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
	camera = EngineInstance::getEngine()->getCamera();
}

void Editor::SceneEditor::update()
{
	DrawSceneHierarchy();
}


void Editor::SceneEditor::DrawSceneHierarchy()
{
	// similar to unity, we want to be able to iterate over every entity in the scene hierarchy.
	// We then want to be able to create and add folders and subfolders, etc. We can't delete the root node
	Entities::EntityList* rootList = currentScene->getRootList();
	DrawHierarchy(rootList);

	// Below is the logic for adding entities and new lists to the hierarchy
	// Right click on our empty space to add a new container outside of an existing container
	if ((selectedContainer || selectedEntity) && ImGui::BeginPopupContextWindow("NewContainer"))
	{
		// create a new folder
		if (selectedContainer)
		{
			// add an entity to this folder
			if (ImGui::MenuItem("Create Entity"))
			{
				Entities::Entity* entity = new Entities::Entity();
				entity->addComponent(new Components::Transform(1, Vector3D(3, 3, 3), 0));
				entity->addComponent(entity->sprite = new Components::Sprite3D());
				entity->sprite->setModel("Cube");
				selectedContainer->add(entity);
			}

			// Add a new folder to the root list
			if (ImGui::MenuItem("Create New Folder"))
			{
				selectedContainer->getSubLists().push_back(new Entities::EntityList());
			}

			// delete a list and all of its sublists
			if (ImGui::MenuItem("Delete Folder"))
			{
				// Delete this container
				Entities::EntityList* parentContainer = FindParent(rootList, selectedContainer);
				if (parentContainer)
				{
					Entities::EntityList* sc = selectedContainer;
					// Find and remove the selected container from its parent's sublists
					auto& sublists = parentContainer->getSubLists();
					auto it = std::find_if(sublists.begin(), sublists.end(),
						[sc](Entities::EntityList* el) { return el == sc; });
					if (it != sublists.end())
					{
						(*it)->clear();
						sublists.erase(it);
					}
					selectedContainer = nullptr;
				}
			}
		}

		if (selectedEntity)
		{
			// delete an entity
			if (ImGui::MenuItem("Delete Entity"))
			{
				Entities::EntityList* parentList = FindEntityList(rootList, selectedEntity);
				if (parentList)
				{
					parentList->remove(selectedEntity);
				}
			}
		}

		ImGui::EndPopup();
	}
	else
	{
		selectedContainer = nullptr;
		selectedEntity = nullptr;
	}
}

// given a list, show every entity in every sublist inside of it; this lets us select it and drag it around
void Editor::SceneEditor::DrawHierarchy(Entities::EntityList* list)
{
	bool nodeOpen = ImGui::TreeNode(list->getName().c_str());

	// hover over our current entity list
	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(1))
	{
		selectedContainer = list;
	}

	// check if the node is open, if it is draw our entities
	if (nodeOpen)
	{
		// Recursively draw sublists
		for (const auto& sublist : list->getSubLists())
		{
			DrawHierarchy(sublist);
		}
		// Draw entities in the current list
		for (const auto& entity : list->getEntities())
		{
			DrawEntity(*entity);
		}

		ImGui::TreePop();
	}
}

// draw the selectable entity in the hierarchy; this lets us inspect it and drag it around
void Editor::SceneEditor::DrawEntity(Entities::Entity& entity)
{
	// draw our entity name, visibility, and locked status
	ImGui::SetNextItemAllowOverlap();

	bool selected = ImGui::Selectable(entity.getName().c_str());
	Textures::TextureLibrary* lib = EngineInstance::getEngine()->getTextureLibrary();

	// select the entity and inspect it
	if (selected)
	{
		Inspector::inspect(&entity);
	}

	// do actions on this entity
	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(1) && !entity.IsLocked())
	{
		selectedEntity = &entity;
	}

	// Calculate position for the buttons
	float cursorPosX = ImGui::GetCursorPosX();
	float windowWidth = ImGui::GetWindowWidth();
	float iconWidth = 24.0f;
	float spacing = 8.0f; // Adjust spacing as needed

	float buttonPosX = windowWidth - (iconWidth + spacing) * 2 - spacing;

	// Align the cursor position to the right for the first button
	ImGui::SameLine(buttonPosX);
	void* visibleIcon = *lib->get("VisibleIcon")->getTextureView();
	void* inVisibleIcon = *lib->get("InvisibleIcon")->getTextureView();
	void* lockedIcon = *lib->get("LockedIcon")->getTextureView();

	void* eye = visibleIcon;

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

	if (!entity.isVisible())
		eye = inVisibleIcon;

	if (ImGui::ImageButton(("##"+std::to_string(entity.GetId())).c_str(), eye, {24,16}))
	{
		entity.ToggleVisiblity();
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	if (entity.IsLocked())
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

		ImGui::SameLine();
		ImGui::ImageButton(lockedIcon, { 16,16 });

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}
}

// Utility function to find the parent of a given entity list
Entities::EntityList* Editor::SceneEditor::FindParent(Entities::EntityList * root, Entities::EntityList * child) 
{
	for (auto& sublist : root->getSubLists()) 
	{
		if (sublist == child) 
		{
			return root;
		}
		Entities::EntityList* found = FindParent(sublist, child);
		if (found) 
		{
			return found;
		}
	}
	return nullptr;
}

Entities::EntityList* Editor::SceneEditor::FindEntityList(Entities::EntityList* root, Entities::Entity* child)
{
	// check for root first
	for (auto& entity : root->getEntities())
	{
		if (entity == child)
			return root;
	}

	// check for sub lists
	for (auto& list : root->getSubLists())
	{
		for (auto& entity : list->getEntities())
		{
			if (entity == child)
				return list;
		}
	}
}

// used to add new entity containers to the scene hierarchy when we right click
void Editor::SceneEditor::interact()
{

}

// code to drag and re-place our entity containers and entities within the scene editor list
void Editor::SceneEditor::DragContainer(Entities::EntityListWrapper* wrapper, int i)
{

}
