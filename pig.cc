#include "pig.hpp"

Pig::Pig(const Vec2i &p, GameMap *m, bool b) : Character(p, m, b){}
Pig::~Pig(){}
string Pig::Move(const Vec2i &, bool){
  return "Oink";
}
