#ifndef MAPSPEC_H
#define MAPSPEC_H
#include <string>
#include <vector>
using namespace std;

class MapSpec {
  unsigned short m_WallProbability;
  unsigned short m_FilterSteps;
  vector<unsigned short> m_Specifiers;
  vector<unsigned short> m_Iterations;
  vector<vector<unsigned short>> m_Radii;
  vector<vector<unsigned short>> m_MinMax;

public:
  MapSpec(const string & = "mapspec.json");
  string ToString() const;
  unsigned short GetWallProbability() const;
  unsigned short GetStepCount() const;
  unsigned short GetSpecifier(const int &) const;
  unsigned short GetIteration(const int &) const;
  unsigned short GetRadius(const int &, const int &) const;
  void GetMinMax(const int &, const int &, int &, int &) const;
};

#endif
