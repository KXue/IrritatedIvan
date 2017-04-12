#ifndef ENTITY_H
#define ENTITY_H

#include "maptype.hpp"
#include "vec2i.hpp"
#include <string>

class Character;
class GameMap;

class Entity {
protected:
  Vec2i m_Position;
  GameMap *m_pMap;
public:
  Entity(const Vec2i&, GameMap* map);
  virtual ~Entity() = 0;
  Vec2i GetPosition() const;

  virtual string Decide();
  virtual string GetName()const = 0 ;
  virtual MapType GetType()const = 0;
  virtual string TakeDamage(const int&) = 0;
  virtual string GetDescription() = 0;
  virtual string GetUsed(Character &user) = 0;
};

#endif
