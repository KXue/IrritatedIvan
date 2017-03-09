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
void Vec2i::GetCoords(int &oX, int &oY) const{
  oX = x;
  oY = y;
}

void Vec2i::operator=(const Vec2i &other){
  x = other.x;
  y = other.y;
}
Vec2i operator+(const Vec2i &first, const Vec2i &second) {
  return Vec2i(first.x + second.x, first.y + second.y);
}
Vec2i operator-(const Vec2i &first, const Vec2i &second) {
  return Vec2i(first.x - second.x, first.y - second.y);
}
int Vec2i::GetIntDistance(const Vec2i &first, const Vec2i &second) {
  return sqrt((first.x - second.x) * (first.x - second.x) +
              (first.y - second.y) * (first.y - second.y));
}
//Bresenham's Line algorithm https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
//TODO: Finish this (perhaps make line class?)
Vec2i Vec2i::LineSegmentIntersectsHorizontal(const Vec2i &first, const Vec2i &second, int y){
  float deltax = second.GetX() - first.GetX();
  float deltay = second.GetY() - first.GetY();
  float deltaerr = abs(deltay / deltax);
  return Vec2i();
}
