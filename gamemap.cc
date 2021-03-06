#include "gamemap.hpp"
#include "mapspec.hpp"
#include "entity.hpp"
#include "character.hpp"
#include "useful.hpp"
#include "vec2i.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <queue>
//Helper functions for raffle draw system
//prints the number of tickets each tile on the map has
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

//Assigns each floor tile with the maximum raffle value
void FirstPass(vector<vector<unsigned int>> &lot, const vector<vector<MapType>> &map, const int &maxRaffle) {
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

//Gets the sum of the raffle tickets of all the tiles.
unsigned int GetRaffleMax(const vector<vector<unsigned int>> &lot) {
  int count = 0;
  for (int row = 0; row < lot.size(); row++) {
    for (int col = 0; col < lot[row].size(); col++) {
      count += lot[row][col];
    }
  }
  return count;
}

//Gets the raffle winner tile.
Vec2i GetRafflePosition(const unsigned int &raffle, const vector<vector<unsigned int>> &lot) {
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
  return position; // SHOULD NEVER HAPPEN
}

int const GameMap::DIRECTIONS[8][2] = {{1, 0},  {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1},  {1, 1}};

void ReassignRaffle(const unsigned int &maxSquareRadius, const int &maxRaffle, const Vec2i &point, vector<vector<unsigned int>> &lot, const int &offset) {
  int r, c;
  int squareRadius;
  int raffle;
  point.GetCoords(c, r);

  for (int row = 0; row < lot.size(); row++) {
    for (int col = 0; col < lot[row].size(); col++) {
      if (lot[row][col] > 0) {
        squareRadius = IntSquareDistance(r, c, row, col);
        if (squareRadius > 0) {
          raffle = max(offset + (maxRaffle) - (int)(maxRaffle * (float)maxSquareRadius / (float)squareRadius), 0);
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
GameMap::GameMap(const MapSpec &spec, RNG *gen) : m_pPlayer(nullptr), m_MaxRaffle(9), m_pGenerator(gen) {
  m_Width = spec.GetWidth();
  m_Height = spec.GetHeight();
  m_Map = vector<vector<MapType>>(m_Height, vector<MapType>(m_Width, MapType::wall));
  m_DistanceMap = vector<vector<int>>(m_Height, vector<int>(m_Width, -1));
  GenerateMap(spec);
}
GameMap::~GameMap() {
  for(int i = 0; i < m_Entities.size(); i++){
    delete m_Entities[i];
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
  vector<vector<MapType>> newMap(m_Height, vector<MapType>(m_Width, MapType::wall));
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
vector<vector<MapType>> GameMap::GenerateRandom(const int &wallPercent) {
  vector<vector<MapType>> newMap(m_Height, vector<MapType>(m_Width, MapType::wall));
  uniform_int_distribution<unsigned char> dist(0, 100);
  for (int row = 0; row < m_Height; row++) {
    for (int col = 0; col < m_Width; col++) {
      if (dist(*m_pGenerator) < wallPercent) {
        newMap[row][col] = MapType::wall;
      } else {
        newMap[row][col] = MapType::floor;
      }
    }
  }
  return newMap;
}
bool GameMap::TryAddEntity(Entity *entity){
  bool success = false;
  if(GetTileAt(entity->GetPosition()) == MapType::floor){
    m_Entities.push_back(entity);
    success = true;
  }
  return success;
}

// Functions used for checking for disconnected bubbles.

// Checks if any bubbles exist
bool GameMap::IsConnected() {
  int row;
  int col;
  uniform_int_distribution<unsigned short> widthDist(0, m_Width - 1);
  uniform_int_distribution<unsigned short> heightDist(0, m_Height - 1);
  do {
    row = heightDist(*m_pGenerator);
    col = widthDist(*m_pGenerator);
  } while (m_Map[row][col] != MapType::floor);
  vector<vector<bool>> traversed = vector<vector<bool>>(m_Height, vector<bool>(m_Width, false));
  unsigned int count = 0;
  FloodArea(row, col, traversed, count);
  return count == TotalArea();
}
// FloodArea and TotalArea should be equal if there are no bubbles
// Recursively floods the map, incrementing a counter each time it hits an untraversed floor.
void GameMap::FloodArea(const int &row, const int &col, vector<vector<bool>> &traversed, unsigned int &count) const {
  if (row < 0 || row >= m_Height || col < 0 || col >= m_Width || traversed[row][col] || m_Map[row][col] != MapType::floor) {
    return;
  } else {
    traversed[row][col] = true;
    count++;
    for (int i = 0; i < 8; i++) {
      FloodArea(row + DIRECTIONS[i][0], col + DIRECTIONS[i][1], traversed, count);
    }
  }
}
// Iterates through the map, incrementing a counter each time it hits an untraversed floor.
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

string GameMap::ToString() const {
  stringstream ss;
  for (int row = 0; row < m_Height; row++) {
    for (int col = 0; col < m_Width; col++) {
      ss << (char)m_Map[row][col];
    }
  }
  string map = ss.str();
  for(auto &element : m_Entities){
    map[GetLineCoord(element->GetPosition())] = (char)element->GetType();
  }
  if(m_pPlayer){
    map[GetLineCoord(m_pPlayer->GetPosition())] = (char)m_pPlayer->GetType();
  }
  map[GetLineCoord(m_EndLocation)] = (char)MapType::downstair;
  for(int i = m_Width * m_Height; i > 0; i -= m_Width){
    map.insert(i, 1, '\n');
  }
  return map;
}
int GameMap::CountWalls(const int &row, const int &col, const int &radius, bool countCentre) const {
  int wallCount = 0;

  int offsetRow;
  int offsetCol;
  for (int r = -radius; r <= radius; r++) {
    for (int c = -radius; c <= radius; c++) {
      offsetRow = row + r;
      offsetCol = col + c;
      if (offsetRow < 0 || offsetRow >= m_Height || offsetCol < 0 || offsetCol >= m_Width || m_Map[offsetRow][offsetCol] == MapType::wall) {
        wallCount++;
      }
    }
  }
  if (!countCentre && m_Map[row][col] == MapType::wall) {
    wallCount--;
  }
  return wallCount;
}
vector<vector<MapType>> GameMap::GetNeighbours(const int &row, const int &col, const int &radius) const {
  vector<vector<MapType>> neighbours(2 * radius + 1, vector<MapType>(2 * radius + 1));
  int offsetRow;
  int offsetCol;
  for (int r = -radius; r <= radius; r++) {
    for (int c = -radius; c <= radius; c++) {
      offsetRow = row + r;
      offsetCol = col + c;
      if (offsetRow < 0 || offsetRow >= m_Height || offsetCol < 0 || offsetCol >= m_Width) {
        neighbours[r][c] = MapType::wall;
      } else {
        neighbours[r][c] = m_Map[offsetRow][offsetCol];
      }
    }
  }
  return neighbours;
}
void GameMap::ResetRaffle(const int &maxRaffle){
  m_MaxRaffle = maxRaffle;
  m_ProbabilityDistribution = vector<vector<unsigned int>>(m_Height, vector<unsigned int>(m_Width, 0));
  FirstPass(m_ProbabilityDistribution, m_Map, maxRaffle);
}
vector<Vec2i> GameMap::RafflePull(const int &numPulls, const int &maxRadius, const int &offset) {
  unsigned int raffle;
  unsigned int raffleMax;
  vector<Vec2i> retVectors;

  for (int i = 0; i < numPulls; i++) {
    raffleMax = GetRaffleMax(m_ProbabilityDistribution);
    if (raffleMax == 0) { // no more open spots. end prematurely
      break;
    }
    //better randoms
    uniform_int_distribution<unsigned int> dist(0, raffleMax - 1);
    raffle = dist(*m_pGenerator);
    Vec2i point = GetRafflePosition(raffle, m_ProbabilityDistribution);
    retVectors.push_back(point);
    ReassignRaffle(maxRadius, m_MaxRaffle, point, m_ProbabilityDistribution, offset);
  }
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

// Game object access and modification

//Gets the tile at position
MapType GameMap::GetTileAt(const Vec2i &point) const {
  int r, c;
  point.GetCoords(c, r);
  MapType retVal = MapType::wall;
  if (r < m_Height && c < m_Width && r >= 0 && c >= 0) {
    retVal = m_Map[r][c];
  }
  if(retVal == MapType::floor){
    for(int i = 0; i < m_Entities.size(); i++){
      if(m_Entities[i]->GetPosition() == point){
        retVal = m_Entities[i]->GetType();
        break;
      }
    }
    if(m_pPlayer && m_pPlayer->GetPosition() == point){
      retVal = m_pPlayer->GetType();
    }
  }
  return retVal;
}
//Gets the entity at a point if it exists, returns false if not found.
bool GameMap::TryGetEntityAt(const Vec2i &point, Entity *&result){
  bool found = false;
  for(int i = 0; i < m_Entities.size(); i++){
    if(point == m_Entities[i]->GetPosition()){
      found = true;
      result = m_Entities[i];
      break;
    }
  }
  if(!found && m_pPlayer->GetPosition() == point){
    found = true;
    result = m_pPlayer;
  }
  return found;
}
bool GameMap::TryRemoveEntityAt(const Vec2i &point){
  bool found = false;
  for(auto it = m_Entities.begin(); it != m_Entities.end(); ++it){
    if(point == (*it)->GetPosition()){
      found = true;
      delete *it;
      m_Entities.erase(it);
      break;
    }
  }
  return found;
}
vector<vector<int>> BreadthFirstGenerate(const vector<vector<MapType>> map, int x, int y){
  int width = map[0].size();
  int height = map.size();

  queue<int> queueX;
  queue<int> queueY;
  queueX.push(x);
  queueY.push(y);
  int qsize = queueX.size();
  vector<vector<bool>>traversed = vector<vector<bool>>(height, vector<bool>(width, false));
  vector<vector<int>>distanceMap = vector<vector<int>>(height, vector<int>(width, -1));

  int distance = 0;
  do{
    for(int j = 0; j < qsize; j++){
      int topX = queueX.front();
      int topY = queueY.front();
      queueX.pop();
      queueY.pop();
      traversed[topY][topX] = true;
      distanceMap[topY][topX] = distance;
      for(int i = 0; i < 8; i++){
        int nextX = topX + GameMap::DIRECTIONS[i][0];
        int nextY = topY + GameMap::DIRECTIONS[i][1];
        if(nextX >= 0 && nextY >= 0 && nextX < width && nextY < height && !traversed[nextY][nextX] && map[nextY][nextX] == MapType::floor){
          traversed[nextY][nextX] = true;
          queueX.push(nextX);
          queueY.push(nextY);
        }
      }
    }
    distance++;
    qsize = queueX.size();
  }while(qsize != 0);

  return distanceMap;
}

void GameMap::UpdateDistanceMap(){
  if(m_pPlayer){
    Vec2i playerPosition = m_pPlayer->GetPosition();
    m_DistanceMap = BreadthFirstGenerate(m_Map, playerPosition.GetX(), playerPosition.GetY());
  }
  // for(int i = 0; i < m_Height; i++){
  //   for(int j = 0; j < m_Width; j++){
  //     if(m_DistanceMap[i][j] > 9){
  //       cout << m_DistanceMap[i][j]%10;
  //     }
  //     else if(m_DistanceMap[i][j] < 0){
  //       cout << '-';
  //     }
  //     else{
  //       cout << m_DistanceMap[i][j];
  //     }
  //   }
  //   cout << endl;
  // }
}
int GameMap::GetDistanceToPlayer(const Vec2i &position){
  int r, c;
  position.GetCoords(c, r);
  return m_DistanceMap[r][c];
}
Vec2i GameMap::DirectionToPlayer(const Vec2i& currentPosition){
  vector<Vec2i> bestDirections;
  int bestDistance = GetDistanceToPlayer(currentPosition);
  for(int i = 0; i < 8; i++){
    Vec2i newDirection = Vec2i(DIRECTIONS[i][0], DIRECTIONS[i][1]);
    Vec2i newPosition = newDirection + currentPosition;
    if((GetTileAt(newPosition) == MapType::floor || GetTileAt(newPosition) == MapType::player)  && !(newPosition == m_EndLocation) ){
      int newDistance = GetDistanceToPlayer(newPosition);
      if(newDistance < bestDistance){
        bestDirections.clear();
        bestDirections.push_back(newDirection);
        bestDistance = newDistance;
      }
      else if(newDistance == bestDistance){
        bestDirections.push_back(newDirection);
      }
    }
  }
  if(bestDirections.size() == 0){
    cout << "WTF???" << endl;
    return Vec2i();
  }
  uniform_int_distribution<unsigned int> dist(0, bestDirections.size() - 1);
  return bestDirections[dist(*m_pGenerator)];
}
Vec2i GameMap::RandomValidDirection(const Vec2i &currentPosition){
  vector<Vec2i> validDirections;
  for(int i = 0; i < 8; i++){
    Vec2i newDirection = Vec2i(DIRECTIONS[i][0], DIRECTIONS[i][1]);
    Vec2i newPosition = newDirection + currentPosition;
    if(GetTileAt(newPosition) == MapType::floor && !(newPosition == m_EndLocation)){
      validDirections.push_back(newDirection);
    }
  }

  uniform_int_distribution<unsigned int> dist(0, validDirections.size() - 1);
  return validDirections[dist(*m_pGenerator)];
}
bool GameMap::TryAddPlayer(Character *player){
  bool retVal = true;
  if(m_pPlayer){
    retVal = false;
  }
  else{
    m_pPlayer = player;
    player->Reposition(m_StartLocation);
    player->ChangeMap(this);
  }
  return retVal;
}
Character * GameMap::TryRemovePlayer(){
  Character *retVal = m_pPlayer;
  if(retVal){
    m_pPlayer = nullptr;
  }
  return retVal;
}
//Getting ugly
Vec2i GameMap::GetStartLocation()const{
  return m_StartLocation;
}
Vec2i GameMap::GetEndLocation()const{
  return m_EndLocation;
}
void GameMap::SetStartLocation(const Vec2i &loc){
  m_StartLocation = loc;
}
void GameMap::SetEndLocation(const Vec2i &loc){
  m_EndLocation = loc;
}
string GameMap::Update(){
  string retVal;
  for(auto &element : m_Entities){
    retVal += element->Decide();
  }
  return retVal;
}
