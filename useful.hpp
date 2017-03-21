#ifndef USEFUL_H
#define USEFUL_H
unsigned int IntSquareDistance(const int &x1, const int &y1, const int &x2,
                               const int &y2) {
  return ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
#endif
