#ifndef ITEM_H
#define ITEM_H
#include "entity.hpp"

class Item : public Entity{
  public:
    virtual string GetName(bool start = false, bool possessive = false)const;
    virtual MapType GetType()const;
    virtual string GetAttacked(const int&, Character &);
    virtual string GetDescription() const;
    virtual string GetUsed(Character &user);
};

#endif
