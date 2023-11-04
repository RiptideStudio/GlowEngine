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
}

namespace Entities
{

  class EntityList
  {

  public:
    EntityList();

    void add(Entities::Entity* entity);
    void update();
    void render();

  private:
    std::vector<Entities::Entity*> activeList;
    std::vector<Entities::Entity*> deferredList;

  };

}