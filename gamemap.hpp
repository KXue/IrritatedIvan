#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "maptype.hpp"
#include "shared.hpp"
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

class Vec2i;
class Character;
class Entity;
class MapSpec;

class GameMap {
  // Map's size will be set once and never be touched again.
  vector<vector<MapType>> m_Map;
  vector<vector<unsigned int>> m_ProbabilityDistribution;
  vector<Entity*> m_Entities;
  vector<vector<int>> m_DistanceMap;
  Character* m_pPlayer;
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
  // TODO: mapwide fill pathfinding
  GameMap(const MapSpec &, RNG *gen);
  ~GameMap();
  string ToString() const;
  int GetLineCoord(const Vec2i &) const;
  MapType GetTileAt(const Vec2i &) const;
  bool TryAddEntity(Entity *entity);
  bool TryGetEntityAt(const Vec2i &point, Entity *result);
  bool RemoveEntityAt(const Vec2i &point);
  bool TryAddPlayer(Character*);
  Character * TryRemovePlayer();
  vector<vector<MapType>> GetNeighbours(const int &, const int &,
                                        const int & = 1) const;
  int CountWalls(const int &, const int &, const int & = 1, bool = false) const;
  void ResetRaffle(const int & = 9);
  vector<Vec2i> RafflePull(const int &, const int &, const int &);
  void UpdateDistanceMap();
  int GetDistanceToPlayer(const Vec2i &);
  Vec2i DirectionToPlayer(const Vec2i &);
};
#endif
