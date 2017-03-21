#include "gamemap.hpp"
#include "mapspec.hpp"
#include "useful.hpp"
#include "vec2i.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>

int const GameMap::DIRECTIONS[8][2] = {{1, 0},  {1, -1}, {0, -1}, {-1, -1},
                                       {-1, 0}, {-1, 1}, {0, 1},  {1, 1}};
void printLot(const vector<vector<unsigned int>> &lot) {
  for (int row = 0; row < lot.size(); row++) {
    for (int col = 0; col < lot[row].size(); col++) {
      if (lot[row][col] > 0) {
        if (lot[row][col] < 10) {
          cout << lot[row][col];
        } else {
          cout << '+';
        }
      } else {
        cout << ' ';
      }
    }
    cout << endl;
  }
}
void FirstPass(vector<vector<unsigned int>> &lot,
               const vector<vector<MapType>> &map, const int &maxRaffle) {
  for (int row = 0; row < lot.size(); row++) {
    for (int col = 0; col < lot[row].size(); col++) {
      if (map[row][col] == MapType::floor) {
        lot[row][col] = maxRaffle;
      } else {
        lot[row][col] = 0;
      }
    }
  }
}

unsigned int GetRaffleMax(const vector<vector<unsigned int>> &lot) {
  int count = 0;
  for (int row = 0; row < lot.size(); row++) {
    for (int col = 0; col < lot[row].size(); col++) {
      count += lot[row][col];
    }
  }
  return count;
}

Vec2i GetRafflePosition(const unsigned int &raffle,
                        const vector<vector<unsigned int>> &lot) {
  Vec2i position = Vec2i(-1, -1);
  unsigned int count = 0;
  for (int row = 0; row < lot.size(); row++) {
    for (int col = 0; col < lot[row].size(); col++) {
      count += lot[row][col];
      if (count >= raffle) {
        position = Vec2i(col, row);
        return position;
      }
    }
  }
  return position; // hopefully doesn't happen
}
void ReassignRaffle(const unsigned int &maxSquareRadius, const int &maxRaffle,
                    const Vec2i &point, vector<vector<unsigned int>> &lot,
                    const int &offset) {
  int r, c;
  int squareRadius;
  int raffle;
  point.GetCoords(c, r);

  for (int row = 0; row < lot.size(); row++) {
    for (int col = 0; col < lot[row].size(); col++) {
      if (lot[row][col] > 0) {
        squareRadius = IntSquareDistance(r, c, row, col);
        if (squareRadius > 0) {
          raffle = max(offset + (maxRaffle) -
                           (int)(maxRaffle * (float)maxSquareRadius /
                                 (float)squareRadius),
                       0);
          if (raffle != 0) {
            if (raffle <= offset) {
              raffle = 1;
            } else {
              raffle -= offset;
            }
          }
          lot[row][col] = min((unsigned int)raffle, lot[row][col]);
        } else {
          lot[row][col] = 0;
        }
      }
    }
  }
}
void GameMap::GenerateMap(const MapSpec &spec) {
  do {
    GenerateFromSpec(spec);
  } while (!IsConnected());
}
void GameMap::GenerateFromSpec(const MapSpec &spec) {
  bool isWall;
  int wallCount = 0;
  int min;
  int max;
  m_Map = GenerateRandom(spec.GetWallProbability());
  cout << ToString() << endl;
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
    cout << ToString() << endl;
  }
}
vector<vector<MapType>> GameMap::GenerateRandom(const int &wallPercent) {
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

/*
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
*/

bool GameMap::IsConnected() {
  int row;
  int col;
  do {
    row = generator() % m_Height;
    col = generator() % m_Width;
  } while (m_Map[row][col] != MapType::floor);
  vector<vector<bool>> traversed =
      vector<vector<bool>>(m_Height, vector<bool>(m_Width, false));
  unsigned int count = 0;
  FloodArea(row, col, traversed, count);
  return count == TotalArea();
}
void GameMap::FloodArea(const int &row, const int &col,
                        vector<vector<bool>> &traversed,
                        unsigned int &count) const {
  if (row < 0 || row >= m_Height || col < 0 || col >= m_Width ||
      traversed[row][col] || m_Map[row][col] != MapType::floor) {
    return;
  } else {
    traversed[row][col] = true;
    count++;
    for (int i = 0; i < 8; i++) {
      FloodArea(row + DIRECTIONS[i][0], col + DIRECTIONS[i][1], traversed,
                count);
    }
  }
}
unsigned short GameMap::TotalArea() const {
  unsigned short area = 0;
  for (int row = 0; row < m_Height; row++) {
    for (int col = 0; col < m_Width; col++) {
      if (m_Map[row][col] == MapType::floor) {
        area++;
      }
    }
  }
  return area;
}

GameMap::GameMap(const MapSpec &spec) {
  m_Width = spec.GetWidth();
  m_Height = spec.GetHeight();
  generator.seed(*spec.GetSeed());
  m_Map = vector<vector<MapType>>(m_Height,
                                  vector<MapType>(m_Width, MapType::wall));
  GenerateMap(spec);
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
vector<Vec2i> GameMap::RafflePull(const int &numPulls, const int &maxRadius,
                                  const int &maxRaffle, const int &offset) {
  unsigned int raffle;
  unsigned int raffleMax;
  vector<Vec2i> retVectors;
  vector<vector<unsigned int>> raffleDistribution =
      vector<vector<unsigned int>>(m_Height, vector<unsigned int>(m_Width, 0));
  FirstPass(raffleDistribution, m_Map, maxRaffle);
  for (int i = 0; i < numPulls; i++) {
    raffleMax = GetRaffleMax(raffleDistribution);
    if (raffleMax == 0) { // no more open spots. end prematurely
      break;
    }
    raffle = generator() % raffleMax;
    Vec2i point = GetRafflePosition(raffle, raffleDistribution);
    retVectors.push_back(point);
    ReassignRaffle(maxRadius, maxRaffle, point, raffleDistribution, offset);
  }
  printLot(raffleDistribution);
  return retVectors;
}
int GameMap::GetLineCoord(const Vec2i &point) const {
  int r, c;
  int retVal = -1;
  point.GetCoords(c, r);
  if (c < m_Width && c >= 0 && r < m_Height && r >= 0) {
    retVal = c + r * m_Width;
  }
  return retVal;
}
