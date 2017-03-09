#include "gamemap.hpp"
//#include "room.hpp"
#include "mapspec.hpp"
#include "vec2i.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {
  MapSpec m;
  cout << m.ToString();
  GameMap map;
  cout << map.ToString();
  return 0;
}
