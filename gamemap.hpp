#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "maptype.hpp"
#include "shared.hpp"
#include "vec2i.hpp"
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

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
  Vec2i m_StartLocation;
  Vec2i m_EndLocation;
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
  string Update();
  int GetLineCoord(const Vec2i &) const;
  MapType GetTileAt(const Vec2i &) const;
  bool TryAddEntity(Entity *entity);
  bool TryGetEntityAt(const Vec2i &point, Entity *&result);
  bool TryRemoveEntityAt(const Vec2i &point);
  bool TryAddPlayer(Character*);
  Character * TryRemovePlayer();
  vector<vector<MapType>> GetNeighbours(const int &, const int &,
                                        const int & = 1) const;
  int CountWalls(const int &, const int &, const int & = 1, bool = false) const;
  void ResetRaffle(const int & = 9);
  vector<Vec2i> RafflePull(const int &, const int &, const int &);
  Vec2i GetStartLocation()const;
  Vec2i GetEndLocation()const;
  void SetStartLocation(const Vec2i&);
  void SetEndLocation(const Vec2i&);
  void UpdateDistanceMap();
  int GetDistanceToPlayer(const Vec2i &);
  Vec2i DirectionToPlayer(const Vec2i &);
  Vec2i RandomValidDirection(const Vec2i &);
};
#endif
