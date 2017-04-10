#ifndef ENTITY_H
#define ENTITY_H

#include "maptype.hpp"
#include "vec2i.hpp"

#include <string>

class Character;
class OutManager;

class Entity {
protected:
  Vec2i m_Position;

public:
  Entity(const Vec2i&);
  virtual ~Entity() = 0;
  Vec2i GetPosition() const;

  virtual string GetName()const = 0 ;
  virtual MapType GetType()const = 0;
  virtual string GetAttacked(const int&, Character &) = 0;
  virtual string GetDescription() const = 0;
  virtual string GetUsed(Character &user) = 0;
};

#endif
