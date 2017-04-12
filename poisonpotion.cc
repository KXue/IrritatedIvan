#include "poisonpotion.hpp"
#include "gamemap.hpp"
#include "character.hpp"
#include "useful.hpp"
#include <sstream>

PoisonPotion::PoisonPotion(const unsigned int &damage, const Vec2i &position, GameMap *map, bool looked) : Item(position, map, looked), m_DamageAmount(damage) {}
PoisonPotion::~PoisonPotion(){}

string PoisonPotion::GetName()const{
  string retVal = "poison potion";
  if(!m_LookedAt){
    retVal = Item::GetName();
  }
  return retVal;
}
MapType PoisonPotion::GetType()const{
  MapType retVal = MapType::poisonpotion;
  if(!m_LookedAt){
    retVal = Item::GetType();
  }
  return retVal;
}
string PoisonPotion::GetDescription(){
  m_LookedAt = true;
  return "It's a poison potion.";
}
string PoisonPotion::GetUsed(Character &user){
  string retVal = user.TakeDamage(m_DamageAmount);
  m_pMap->RemoveEntityAt(GetPosition());
  return retVal;
}
