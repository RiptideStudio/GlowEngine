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
  for (auto it = activeList.begin(); it != activeList.end(); )
  {
    // check for destroyed entities
    if ((*it)->isDestroyed())
    {
      destroyList.push_back(*it);
      // erase the entity from the active list and get the new iterator position
      it = activeList.erase(it);
    }
    else
    {
      // update the entity and increment the iterator
      (*it)->update();
      ++it;
    }
  }

  // destroy entities
  for (auto& entity : destroyList)
  {
    entity->~Entity();
    delete entity;
  }
  destroyList.clear();
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

Entities::Entity* Entities::EntityList::find(std::string name)
{
  for (auto entity : activeList)
  {
    if (entity->getName() == name)
    {
      return entity;
    }
  }
  return nullptr;
}
