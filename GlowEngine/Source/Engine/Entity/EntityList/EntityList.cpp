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

  // update all entities in the active list
  checkCollisions();

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
    if (!entity->isVisible())
      continue;

    entity->render();
  }
}

void Entities::EntityList::clear()
{
  activeList.clear();
}

void Entities::EntityList::checkCollisions()
{
  for (auto it1 = activeList.begin(); it1 != activeList.end(); ++it1) 
  {
    Entity* ent1 = *it1;
    Components::Collider* collider1 = getComponentOfType(Collider, ent1);

    if (!collider1) 
      continue;

    for (auto it2 = std::next(it1); it2 != activeList.end(); ++it2) 
    {
      Entity* ent2 = *it2;
      Components::Collider* collider2 = getComponentOfType(Collider, ent2);

      if (!collider2) 
        continue;

      // resolve collisions
      if (collider1->isColliding(collider2)) 
      {
        collider2->onCollide(collider1);
        collider1->onCollide(collider2);
      }
      else
      {
        if (collider1->collidingObjects.find(collider2) != collider1->collidingObjects.end())
        {
          collider2->onLeaveCollide(collider1);
          collider1->onLeaveCollide(collider2);
        }
      }
    }
  }
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
