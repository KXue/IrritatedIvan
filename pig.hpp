#ifndef PIG_H
#define PIG_H
#include "character.hpp"

class Pig : public Character{
public:
  Pig(const Vec2i&, GameMap*, bool);
  ~Pig();
  string GetName()const;
  MapType GetType()const;
  string GetDescription();
};

#endif
