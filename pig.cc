#include "pig.hpp"

Pig::Pig(const Vec2i &p, GameMap *m, bool b) : Character(p, m, b){}
Pig::~Pig(){}
string Pig::Move(const Vec2i &direction, bool){
  SetLastAction(&Character::Move, direction);
  return "Oink";
}
