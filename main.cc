#include "gamemap.hpp"
#include "mapspec.hpp"
#include "vec2i.hpp"

#include "game.hpp"
//#include "character.hpp"
#include <iostream>
bool isQuit = false;
void quit(){
  isQuit = true;
}
int main(int argc, char const *argv[]) {
  cout << "START" << endl;
  Game game;
  game.Start();
  return 0;
}
