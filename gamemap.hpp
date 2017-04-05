#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "entity.hpp"
#include "mapspec.hpp"
#include "maptype.hpp"
#include "shared.hpp"
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

class Vec2i;

class GameMap {
  // Map's size will be set once and never be touched again.
  vector<vector<MapType>> m_Map;
  vector<vector<unsigned int>> m_ProbabilityDistribution;
  vector<Entity*> m_Entities;
  int m_Width;
  int m_Height;
  unsigned int m_MaxRaffle;
  RNG* m_pGenerator;

  // member functions
  void GenerateMap(const MapSpec &);
  void GenerateFromSpec(const MapSpec &n);
  vector<vector<MapType>> GenerateRandom(const int &wallPercent);
  bool IsConnected();
  void FloodArea(const int &, const int &, vector<vector<bool>> &,
                 unsigned int &) const;
  unsigned short TotalArea() const;
public:
  static int const DIRECTIONS[8][2];
  GameMap(const MapSpec &, RNG *gen);
  ~GameMap();
  string ToString() const;
  int GetLineCoord(const Vec2i &) const;
  MapType GetTileAt(const Vec2i &) const;
  bool TryAddEntity(Entity *entity);
  bool TryGetEntityAt(const Vec2i &point, Entity *result);
  bool RemoveEntityAt(const Vec2i &point);
  vector<vector<MapType>> GetNeighbours(const int &, const int &,
                                        const int & = 1) const;
  int CountWalls(const int &, const int &, const int & = 1, bool = false) const;
  void ResetRaffle(const int & = 9);
  vector<Vec2i> RafflePull(const int &, const int &, const int &);
};
#endif
