#ifndef VEC2I_H
#define VEC2I_H
#include <string>

using namespace std;

class Vec2i {
  int x;
  int y;

public:
  Vec2i();
  Vec2i(int, int);
  Vec2i(const Vec2i &);
  void operator=(const Vec2i &);
  string ToString() const;
  int GetX() const;
  int GetY() const;
  void GetCoords(int &, int &) const;
  friend Vec2i operator+(const Vec2i &, const Vec2i &);
  friend Vec2i operator-(const Vec2i &, const Vec2i &);
  static int GetIntDistance(const Vec2i &, const Vec2i &);
  Vec2i LineSegmentIntersectsHorizontal(const Vec2i &, const Vec2i &, int y);
};

#endif
