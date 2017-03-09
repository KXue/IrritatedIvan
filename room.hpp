#ifndef ROOM_H
#define ROOM_H
#include "vec2i.hpp"
#include <string>
#include <vector>

using namespace std;

class Room {
  vector<Vec2i> corners;

public:
  Room();
  Room(const Vec2i[], int size);
  Room(const Room &);
  //~Room();
  void AddCorner(const Vec2i &);
  bool IsInside(const Vec2i &);
  string ToString();
};
#endif
