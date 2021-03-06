#ifndef USEFUL_H
#define USEFUL_H
#include <string>
#include <vector>
#include "vec2i.hpp"

unsigned int IntSquareDistance(const int &x1, const int &y1, const int &x2,
                               const int &y2);

std::string ApplyPossessive(std::string base, char perspective);

std::string Pluralize(std::string base, bool fishes);
std::string Capitalize(std::string base);

void PopFurthestPoints(std::vector<Vec2i>&,Vec2i&, Vec2i&);

#endif
