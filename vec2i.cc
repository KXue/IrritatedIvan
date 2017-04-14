#include "vec2i.hpp"
#include <cmath>
#include <sstream>

using namespace std;

Vec2i::Vec2i() : Vec2i(0, 0) {}
Vec2i::Vec2i(int x, int y) : x(x), y(y) {}
Vec2i::Vec2i(const Vec2i &other) : x(other.x), y(other.y) {}
string Vec2i::ToString() const{
  stringstream ss;
  ss << x << ", " << y;
  return ss.str();
}
int Vec2i::GetX() const { return x; }
int Vec2i::GetY() const { return y; }
void Vec2i::GetCoords(int &oX, int &oY) const {
  oX = x;
  oY = y;
}

void Vec2i::operator=(const Vec2i &other) {
  x = other.x;
  y = other.y;
}

bool Vec2i::operator==(const Vec2i &other) const {
  return (x == other.x && y == other.y);
}

Vec2i Vec2i::operator+(const Vec2i &other) const {
  return Vec2i(x + other.x, y + other.y);
}

Vec2i Vec2i::operator-(const Vec2i &other) const {
  return Vec2i(x - other.x, y - other.y);
}
int Vec2i::GetIntDistance(const Vec2i &first, const Vec2i &second){
  return sqrt((first.x - second.x) * (first.x - second.x) +
              (first.y - second.y) * (first.y - second.y));
}
int Vec2i::GetIntSquareDistance(const Vec2i &first, const Vec2i &second){
  return (first.x - second.x) * (first.x - second.x) +
          (first.y - second.y) * (first.y - second.y);
}
