#include "rectangle.hpp"

using namespace std;

Rectangle::Rectangle(const Vec2i & topLeft, const Vec2i & size) : topLeft(topLeft), size(size){}
Vec2i Rectangle::GetTopLeft() const{
    return topLeft;
}
Vec2i Rectangle::GetSize() const{
    return size;
}
void Rectangle::SetTopLeft(Vec2i newPos){
    topLeft = newPos;
}
void Rectangle::SetSize(Vec2i newSize){
    size = newSize;
}
bool Rectangle::ContainsPoint(const Vec2i &point, bool considerWalls) const{
    int x = point.GetX();
    int y = point.GetY();
    Vec2i max = topLeft + size;
    bool isInside = false;
    if(considerWalls){
        isInside = (x >= topLeft.GetX() && x <= max.GetX() &&
                    y >= topLeft.GetY() && y <= max.GetY());
    }else{
        isInside = (x > topLeft.GetX() && x < max.GetX() &&
                    y > topLeft.GetY() && y < max.GetY());
    }
    return isInside;
}
// Rectangle Rectangle::GetIntersection(const Rectangle &first, const Rectangle &second){
//     if(Rectangle::CheckIntersection(first, second)){

//     }
// }
// bool Rectangle::CheckIntersection(const Rectangle &first, const Rectangle &second){
//     bool intersectionFound = false;
//     Vec2i firstMax = first.topLeft + first.size;
//     int firstSmallestX = first.topLeft.GetX();
//     int firstLargestX = firstMax.GetX();
//     int firstSmallestY = first.topLeft.GetY();
//     int firstLargestY = firstMax.GetY();

//     Vec2i secondMax = second.topLeft + second.size;
//     int secondSmallestX = second.topLeft.GetX();
//     int secondLargestX = secondMax.GetX();
//     int secondSmallestY = second.topLeft.GetY();
//     int secondLargestY = secondMax.GetY();

// }
void Rectangle::CalculateArea(){
    area = size.GetX() * size.GetY();
}