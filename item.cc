#include "item.hpp"
#include "gamemap.hpp"

Item::Item(const Vec2i&position, GameMap *map, bool looked) : Entity(position, map), m_LookedAt(looked){}
Item::~Item(){};
string Item::GetName()const{
  return "unknown item";
}
MapType Item::GetType()const{
  return MapType::unknown;
}
string Item::TakeDamage(const int&){
  string retVal = (GetName() + " is destroyed");
  m_pMap->TryRemoveEntityAt(GetPosition());
  return retVal;
}
