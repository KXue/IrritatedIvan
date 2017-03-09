#include "room.hpp"
#include <iostream>
#include <sstream>
using namespace std;

Room::Room() : Room({}, 0) {}
Room::Room(const Vec2i points[], int size) : corners(vector<Vec2i>()) {
  for (int i = 0; i < size; i++) {
    corners.push_back(Vec2i(points[i]));
  }
}
Room::Room(const Room &other) : corners(other.corners) {}
string Room::ToString() {
  int size = corners.size();
  stringstream ss;
  ss << "Corners: " << endl;
  for (int i = 0; i < size; i++) {
    ss << corners[i].GetX() << ", " << corners[i].GetY() << endl;
  }
  return ss.str();
}
void Room::AddCorner(const Vec2i &point) { corners.push_back(Vec2i(point)); }

bool Room::IsInside(const Vec2i &point){
  int intersectCount = 0;
  int size = corners.size();
  for(int i = 0; i < size; i++){
    if(Vec2i::LineSegmentIntersectsHorizontal(corners[i], corners[(i+1)%size], point.GetY())){
      intersectCount++;
    }
  }
  return (intersectCount % 2 == 0) && intersectCount != 0;
}

