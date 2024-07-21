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
#include "Engine/GlowEngine.h"
#include "Game/Scene/Scene.h"
#include "Game/Scene/SceneSystem.h"

// base constructor
Entities::EntityList::EntityList()
:
  size(0),
  name("Container")
{
  
}

// add a given entity to the list
void Entities::EntityList::add(Entities::Entity* entity)
{
  // ensure no duplicate named entities
  if (entity->getName() == "Entity")
  {
    entity->setName("Entity"+std::to_string(activeList.size()));
  }

  activeList.push_back(entity);
  size++;
}

// update a list of entities
void Entities::EntityList::update()
{
  // update our parent scene to the current scene
  parentScene = EngineInstance::getEngine()->getSceneSystem()->getCurrentScene();

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

// this function is used to update our global list and check for collisions
void Entities::EntityList::updateColliders()
{
  checkCollisions();

  colliderList.clear();
  nonStaticList.clear();
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
  for (auto it1 = nonStaticList.begin(); it1 != nonStaticList.end(); ++it1)
  {
    Entity* ent1 = *it1;
    Components::Collider* collider1 = getComponentOfType(Collider, ent1);

    for (auto it2 = colliderList.begin(); it2 != colliderList.end(); ++it2)
    {
      Entity* ent2 = *it2;

      // don't collide with ourselves; this seems strange, but this allows us to have absurd amounts
      // of static colliders with almost zero drawback
      if (ent2 == ent1)
        continue;

      Components::Collider* collider2 = getComponentOfType(Collider, ent2);

      // if both colliders are static, then we don't want to do any collision logic
      // this reduces our collision to O(N) complexity  
      if (collider2->isStatic() && collider1->isStatic()) 
        break;

      // detect collisions
      if (collider1->isColliding(collider2)) 
      {
        collider2->updateCollision(collider1);
        collider1->updateCollision(collider2);
      }
      else
      {
        if (collider1->getCollidingObjects().find(collider2) != collider1->getCollidingObjects().end() 
          && collider1->hasCollided() 
          && collider2->hasCollided())
        {
          collider2->leaveCollision(collider1);
          collider1->leaveCollision(collider2);
        }
      }
    }
  }
  colliderList.clear();
  nonStaticList.clear();
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
