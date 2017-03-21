#include "gamemap.hpp"
#include "mapspec.hpp"
#include "vec2i.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {
  MapSpec m;
  cout << m.ToString();
  GameMap map(m);
  cout << map.ToString();
  vector<Vec2i> points = map.RafflePull(20, 100, 9, 20);
  for (int i = 0; i < points.size(); i++) {
    cout << points[i].ToString() << endl;
  }
  return 0;
}
