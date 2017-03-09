#include "gamemap.hpp"
#include "mapspec.hpp"
#include <iostream>
#include <random>
#include <sstream>

int const GameMap::DIRECTIONS[8][2] = {{1, 0},  {1, -1}, {0, -1}, {-1, -1},
                                       {-1, 0}, {-1, 1}, {0, 1},  {1, 1}};

void GameMap::GenerateMap(bool isCellular) {
  m_Map = vector<vector<MapType>>(m_Height,
                                  vector<MapType>(m_Width, MapType::wall));
  if (isCellular) {
    MapSpec mapspec;
    GenerateFromSpec(mapspec);
  } else {
    MinerGenerate();
  }
}
void GameMap::GenerateFromSpec(const MapSpec &spec) {
  bool isWall;
  int wallCount = 0;
  int min;
  int max;
  m_Map = GenerateRandom(spec.GetWallProbability());
  vector<vector<MapType>> newMap(m_Height,
                                 vector<MapType>(m_Width, MapType::wall));
  unsigned short filterSteps = spec.GetStepCount();
  for (int i = 0; i < filterSteps; i++) {
    for (int j = 0; j < spec.GetIteration(i); j++) {
      for (int row = 0; row < m_Height; row++) {
        for (int col = 0; col < m_Width; col++) {

          isWall = false;

          for (int k = 0; k < spec.GetSpecifier(i); k++) {
            wallCount = CountWalls(row, col, spec.GetRadius(i, k), true);
            spec.GetMinMax(i, k, min, max);
            isWall |= (wallCount >= min && wallCount <= max);
            if (isWall) {
              newMap[row][col] = MapType::wall;
              break;
            }
          }

          if (!isWall) {
            newMap[row][col] = MapType::floor;
          }
        }
      }
      m_Map = newMap;
    }
  }
}
vector<vector<MapType>> GameMap::GenerateRandom(const int &wallPercent) const {
  minstd_rand0 generator;
  vector<vector<MapType>> newMap(m_Height,
                                 vector<MapType>(m_Width, MapType::wall));
  for (int row = 0; row < m_Height; row++) {
    for (int col = 0; col < m_Width; col++) {
      if (generator() % 100 < wallPercent) {
        newMap[row][col] = MapType::wall;
      } else {
        newMap[row][col] = MapType::floor;
      }
    }
  }
  return newMap;
}
vector<vector<MapType>>
GameMap::CellularAutomataFilterStep(const int minMaxArray[][2],
                                    const int radii[], const size_t &n) const {
  bool isWall = false;
  int wallCount = 0;
  vector<vector<MapType>> newMap(m_Height,
                                 vector<MapType>(m_Width, MapType::wall));

  for (int row = 0; row < m_Height; row++) {
    for (int col = 0; col < m_Width; col++) {
      isWall = false;

      for (int j = 0; j < n; j++) {
        wallCount = CountWalls(row, col, radii[j], true);
        isWall |=
            (wallCount >= minMaxArray[j][0] && wallCount <= minMaxArray[j][1]);
        if (isWall) {
          newMap[row][col] = MapType::wall;
          break;
        }
      }

      if (!isWall) {
        newMap[row][col] = MapType::floor;
      }
    }
  }
  return newMap;
}

void GameMap::CellularAutomataGenerate(const int &wallPercentage,
                                       const int &roughIterations,
                                       const int &smoothIterations) {
  minstd_rand0 generator;
  vector<vector<MapType>> newMap(m_Height,
                                 vector<MapType>(m_Width, MapType::wall));
  for (int row = 0; row < m_Height; row++) {
    for (int col = 0; col < m_Width; col++) {
      if (generator() % 100 < 45) {
        newMap[row][col] = MapType::wall;
      } else {
        newMap[row][col] = MapType::floor;
      }
    }
  }
  m_Map = newMap;
  for (int i = 0; i < roughIterations; i++) {
    for (int row = 0; row < m_Height; row++) {
      for (int col = 0; col < m_Width; col++) {
        if (CountWalls(row, col, 1, true) >= 5 ||
            CountWalls(row, col, 2, true) <= 1) {
          newMap[row][col] = MapType::wall;
        } else {
          newMap[row][col] = MapType::floor;
        }
      }
    }
    m_Map = newMap;
  }
  for (int i = 0; i < smoothIterations; i++) {
    for (int row = 0; row < m_Height; row++) {
      for (int col = 0; col < m_Width; col++) {
        if (CountWalls(row, col, 1, true) >= 5) {
          newMap[row][col] = MapType::wall;
        } else {
          newMap[row][col] = MapType::floor;
        }
      }
    }
    m_Map = newMap;
  }
}
void GameMap::MinerGenerate() {}

GameMap::GameMap(const int &m_Width, const int &m_Height)
    : m_Width(m_Width), m_Height(m_Height) {
  GenerateMap();
}
GameMap::~GameMap() {}
string GameMap::ToString() const {
  stringstream ss;
  for (int row = 0; row < m_Height; row++) {
    for (int col = 0; col < m_Width; col++) {
      ss << (char)m_Map[row][col];
    }
    ss << endl;
  }
  return ss.str();
}
int GameMap::CountWalls(const int &row, const int &col, const int &radius,
                        bool countCentre) const {
  int wallCount = 0;

  int offsetRow;
  int offsetCol;
  for (int r = -radius; r <= radius; r++) {
    for (int c = -radius; c <= radius; c++) {
      offsetRow = row + r;
      offsetCol = col + c;
      if (offsetRow < 0 || offsetRow >= m_Height || offsetCol < 0 ||
          offsetCol >= m_Width ||
          m_Map[offsetRow][offsetCol] == MapType::wall) {
        wallCount++;
      }
    }
  }
  if (!countCentre && m_Map[row][col] == MapType::wall) {
    wallCount--;
  }
  return wallCount;
}
vector<vector<MapType>> GameMap::GetNeighbours(const int &row, const int &col,
                                               const int &radius) const {
  vector<vector<MapType>> neighbours(2 * radius + 1,
                                     vector<MapType>(2 * radius + 1));
  int offsetRow;
  int offsetCol;
  for (int r = -radius; r <= radius; r++) {
    for (int c = -radius; c <= radius; c++) {
      offsetRow = row + r;
      offsetCol = col + c;
      if (offsetRow < 0 || offsetRow >= m_Height || offsetCol < 0 ||
          offsetCol >= m_Width) {
        neighbours[r][c] = MapType::wall;
      } else {
        neighbours[r][c] = m_Map[offsetRow][offsetCol];
      }
    }
  }
  return neighbours;
}
