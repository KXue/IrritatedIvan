#include "healthpotion.hpp"
#include "gamemap.hpp"
#include "character.hpp"
#include "useful.hpp"
#include <sstream>

HealthPotion::HealthPotion(const unsigned int &healAmount, const Vec2i &position, GameMap *map, bool looked) : Item(position, map, looked), m_HealAmount(healAmount) {}
HealthPotion::~HealthPotion(){}

string HealthPotion::GetName()const{
  string retVal = "health potion";
  if(!m_LookedAt){
    retVal = Item::GetName();
  }
  return retVal;
}
MapType HealthPotion::GetType()const{
  MapType retVal = MapType::healthpotion;
  if(!m_LookedAt){
    retVal = Item::GetType();
  }
  return retVal;
}
string HealthPotion::GetDescription(){
  m_LookedAt = true;
  return "It's a health potion.";
}
string HealthPotion::GetUsed(Character &user){
  unsigned int healAmount = user.Heal(m_HealAmount);
  stringstream ss;
  ss << Capitalize(user.GetName()) << " heals for " << healAmount << " health!" << endl;
  m_pMap->RemoveEntityAt(GetPosition());
  return ss.str();
}