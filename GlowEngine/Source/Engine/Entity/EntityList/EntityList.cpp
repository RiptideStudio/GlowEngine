/*
/
// filename: EntityList.cpp
// author: Callen Betts
// brief: implements EntityList.h
//
// description: Entity lists let us create containers of entities which will be rendered and 
// updated each frame.
/
*/

#include "stdafx.h"
#include "EntityList.h"
#include "Engine/Entity/Entity.h"

// base constructor
Entities::EntityList::EntityList()
{
  size = 0;
}

// add a given entity to the list
void Entities::EntityList::add(Entities::Entity* entity)
{
  activeList.push_back(entity);
  size++;
}

// update a list of entities
void Entities::EntityList::update()
{
  // update all entities in the active list
  for (auto entity : activeList)
  {
    entity->update();
  }
}

// update a list of entities
void Entities::EntityList::render()
{
  // render all entities in the list
  for (auto entity : activeList)
  {
    entity->render();
  }
}

void Entities::EntityList::clear()
{
  activeList.clear();
}
