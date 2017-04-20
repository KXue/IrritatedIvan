#ifndef PIG_H
#define PIG_H
#include "character.hpp"

class Pig : public Character{
public:
  Pig(const Vec2i&, GameMap*, bool, const unsigned int & = 5,
            const unsigned short & = 3, const unsigned short & = 2,
            const unsigned char & = 1);
  ~Pig();
  string GetName()const;
  MapType GetType()const;
  string GetDescription();
};

#endif
