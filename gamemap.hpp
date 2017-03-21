#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "mapspec.hpp"
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

enum class MapType : char { wall = '#', floor = '.' };

class Vec2i;

class GameMap {
  // Map's size will be set once and never be touched again.
  vector<vector<MapType>> m_Map;
  int m_Width;
  int m_Height;
  mt19937 generator;

  // member functions
  void GenerateMap(const MapSpec &);
  void GenerateFromSpec(const MapSpec &);
  vector<vector<MapType>> GenerateRandom(const int &wallPercent);
  bool IsConnected();
  void FloodArea(const int &, const int &, vector<vector<bool>> &,
                 unsigned int &) const;
  unsigned short TotalArea() const;

  // Not yet implemented
  // void MinerGenerate();
  /*
  // no longer used.
  void CellularAutomataGenerate(const int & = 40, const int & = 4,
                                const int & = 3);
  // no longer used.
  vector<vector<MapType>> CellularAutomataFilterStep(const int minMaxArray[][2],
                                                     const int[],
                                                     const size_t &) const;
  */
public:
  static int const DIRECTIONS[8][2];
  GameMap(const MapSpec &);
  ~GameMap();
  string ToString() const;
  int GetLineCoord(const Vec2i &) const;
  vector<vector<MapType>> GetNeighbours(const int &, const int &,
                                        const int & = 1) const;
  int CountWalls(const int &, const int &, const int & = 1, bool = false) const;
  vector<Vec2i> RafflePull(const int &, const int &, const int &, const int &);
};
#endif
