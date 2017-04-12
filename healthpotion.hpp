#ifndef HPOTION_H
#define HPOTION_H

#include "item.hpp"

class HealthPotion : public Item{
  unsigned int m_HealAmount;
public:
  HealthPotion(const unsigned int&, const Vec2i&, GameMap*, bool = false);
  ~HealthPotion();
  virtual string GetName()const;
  virtual MapType GetType()const;
  virtual string GetDescription();
  virtual string GetUsed(Character &user);
};

#endif
