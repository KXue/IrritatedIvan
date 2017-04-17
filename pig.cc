#include "pig.hpp"
#include <iostream>
Pig::Pig(const Vec2i &p, GameMap *m, bool b) : Character(p, m, b){}
Pig::~Pig(){}
string Pig::GetName()const{
  if(m_IsPlayer){
    return "you";
  }
  return "pig";
}
MapType Pig::GetType()const{
  if(m_IsPlayer){
    return MapType::player;
  }
  return MapType::pig;
}
