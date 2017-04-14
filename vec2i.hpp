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
  bool operator==(const Vec2i&)const;
  string ToString() const;
  int GetX() const;
  int GetY() const;
  void GetCoords(int &, int &) const;
  Vec2i operator+(const Vec2i &)const;
  Vec2i operator-(const Vec2i &)const;
  static int GetIntDistance(const Vec2i &, const Vec2i &);
  static int GetIntSquareDistance(const Vec2i &, const Vec2i &);
};
#endif
