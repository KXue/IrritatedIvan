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
  Game game(3);
  game.Start();
  return 0;
}
