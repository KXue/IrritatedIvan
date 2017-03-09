#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "vec2i.hpp"
// Rectangles currently mainly used as "subrooms" used to create unique room geometry
// A rectangle of size 0, 0 will have no walkable space and be invisible
// A rectangle of size 1, 1 will have no walkable space and be a 1x1 square of wall.
// A rectangle of size 2, 2 will have no walkable space and be a 2x2 square of wall.
// A rectangle of size 3, 3 will finally have a 1x1 walkable square and contain 8 walls.
// Default size is 3x3.

class Rectangle{
    Vec2i topLeft;
    Vec2i size;
    int area;
    void CalculateArea();
public:
    Rectangle(const Vec2i & = Vec2i(0, 0), const Vec2i & = Vec2i(3, 3));
    Vec2i GetTopLeft() const;
    Vec2i GetSize() const;
    void SetTopLeft(Vec2i);
    void SetSize(Vec2i);
    //Set bool to true to consider walls as inside. False to only consider walkable points.
    bool ContainsPoint(const Vec2i &, bool)const;
    // static Rectangle GetIntersection(const Rectangle &, const Rectangle &);
    // static bool CheckIntersection(const Rectangle &, const Rectangle &);
};
#endif