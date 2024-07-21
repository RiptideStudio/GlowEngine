/*
/
// filename: EntityList.h
// author: Callen Betts
// brief: defines base EntityList class as a container of entities
/
*/

#pragma once

namespace Entities
{
  class Entity;
  class EntityList;

  // define a wrapper for entity lists
  class EntityListWrapper
  {

  public:

    EntityListWrapper(Entities::EntityList* newList = nullptr) { list = newList; name = "Wrapper"; }

    // pointer to our list
    Entities::EntityList* list;

    // if the list is collapsed
    bool collapsed = true;
    // if the container is visible or active
    bool active = true;
    // name of wrapper
    std::string name;

    // parent wrapper
    EntityListWrapper* parent;

  };

  class EntityList
  {

  public:
    EntityList();

    void add(Entities::Entity* entity);
    void update();
    void updateColliders();
    void render();
    void clear();

    void checkCollisions();

    Entities::Entity* find(std::string name);

    int getSize() { return size; }
    std::string getName() { return name; }

    // retrieve the active list; one returns const while one does not, so our scene editor can modify it
    std::vector<Entities::Entity*>& getEntities() { return activeList; }
    const std::vector<Entities::Entity*>& getEntities() const { return activeList; }

    std::vector<Entities::Entity*>& getColliderList() { return colliderList; }
    std::vector<Entities::Entity*>& getNonStaticList() { return nonStaticList; }

  private:

    // vectors of lists for handling updates and collisions
    std::vector<Entities::Entity*> activeList;
    std::vector<Entities::Entity*> destroyList;

    std::vector<Entities::Entity*> colliderList;
    std::vector<Entities::Entity*> nonStaticList;

    // pointer to our parent scene
    Scene::Scene* parentScene = nullptr;

    // size of current active list
    int size;
    std::unordered_map<std::string, int> nameCount;

    // name of entity list for display purposes
    std::string name;

  };

}