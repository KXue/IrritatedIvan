#include "pig.hpp"
#include <iostream>
Pig::Pig(const Vec2i &p, GameMap *m, bool b) : Character(p, m, b){
  cout << m_pMap << endl;
  cout << m << endl;
}
Pig::~Pig(){}
