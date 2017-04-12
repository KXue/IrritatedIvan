#ifndef ITEM_H
#define ITEM_H
#include "entity.hpp"

class Item : public Entity{
public:
  Item(const Vec2i&, GameMap*, bool = false);
  virtual ~Item() = 0;
  virtual string GetName()const;
  virtual MapType GetType()const;
  virtual string TakeDamage(const int&);
  virtual string GetDescription() = 0;
  virtual string GetUsed(Character &user) = 0;
protected:
  bool m_LookedAt;
};

#endif
