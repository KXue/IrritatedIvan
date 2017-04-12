#ifndef PPOTION_H
#define PPOTION_H

#include "item.hpp"

class PoisonPotion : public Item{
  unsigned int m_DamageAmount;
public:
  PoisonPotion(const unsigned int&, const Vec2i&, GameMap*, bool = false);
  ~PoisonPotion();
  virtual string GetName()const;
  virtual MapType GetType()const;
  virtual string GetDescription();
  virtual string GetUsed(Character &user);
};

#endif
