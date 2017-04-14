#include "useful.hpp"
unsigned int IntSquareDistance(const int &x1, const int &y1, const int &x2,
                               const int &y2) {
  return ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

std::string ApplyPossessive(std::string base, char perspective){
  if(perspective == 1){ // first person
    base = "my";
  }
  else if(perspective == 2){ // second person
    base += "'re";
  }else if(perspective == 3){ // third person
    base += "'s";
  }
  return base;
}

std::string Pluralize(std::string base, bool fishes){
  if(fishes){
    base += "es";
  }
  return base += "s";
}

std::string Capitalize(std::string base){
  base[0] = toupper(base[0]);
  return base;
}

std::string Verbalize(std::string base, char perspective){
  std::string isExceptionArray[3] = {"am", "are", "is"};
  if(base == "is" || base == "are" || base == "am"){
    base = isExceptionArray[(int)perspective];
  }
  else if(perspective > 2){
    base += 's';
  }
  return base;
}

void PopFurthestPoints(std::vector<Vec2i> &points, Vec2i &first, Vec2i &second){
  unsigned int maxSquareDistance = 0;
  vector<Vec2i>::iterator firstIt, secondIt;
  for(auto it = points.begin(); it != points.end(); ++it){
    for(auto jt = points.begin(); jt != points.end(); ++jt){
      unsigned int squareDistance = Vec2i::GetIntSquareDistance(*it, *jt);
      if(squareDistance > maxSquareDistance){
        maxSquareDistance = squareDistance;
        firstIt = it;
        secondIt = jt;
      }
    }
  }
  first = Vec2i(*firstIt);
  second = Vec2i(*secondIt);
  points.erase(secondIt);
  points.erase(firstIt);
}
