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
string Pig::GetDescription(){
  return "This is a hostile pig. You're not sure what you did to make it hostile but it's hostile. ";
}
