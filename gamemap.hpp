#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "mapspec.hpp"
#include <string>
#include <vector>

using namespace std;

enum class MapType : char { wall = '#', floor = '.' };

class GameMap {
  static int const DIRECTIONS[8][2];
  // Map's size will be set once and never be touched again.
  vector<vector<MapType>> m_Map;
  int m_Width;
  int m_Height;

  // member functions
  void GenerateMap(bool = true);
  void GenerateFromSpec(const MapSpec &);
  vector<vector<MapType>> GenerateRandom(const int &wallPercent) const;
  void CellularAutomataGenerate(const int & = 40, const int & = 4,
                                const int & = 3);
  vector<vector<MapType>> CellularAutomataFilterStep(const int minMaxArray[][2],
                                                     const int[],
                                                     const size_t &) const;
  void MinerGenerate();

public:
  GameMap(const int & = 80, const int & = 100);
  ~GameMap();
  string ToString() const;
  vector<vector<MapType>> GetNeighbours(const int &, const int &,
                                        const int & = 1) const;
  int CountWalls(const int &, const int &, const int & = 1, bool = false) const;
};
#endif
