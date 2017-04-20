#include "pig.hpp"
#include <iostream>
Pig::Pig(const Vec2i &p, GameMap *m, bool player, const unsigned int & maxhealth, const unsigned short &attack, const unsigned short &defense, const unsigned char &actions) : Character(p, m, player, maxhealth, attack, defense, actions){}
Pig::~Pig(){}
string Pig::GetName()const{
  if(m_IsPlayer){
    return Character::GetName();
  }
  return "pig";
}
MapType Pig::GetType()const{
  if(m_IsPlayer){
    return Character::GetType();
  }
  return MapType::pig;
}
string Pig::GetDescription(){
  return "This is a hostile pig. You're not sure what you did to make it hostile but it's hostile. ";
}
