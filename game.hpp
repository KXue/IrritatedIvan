#ifndef GAME_H
#define GAME_H
#include "mapspec.hpp"
#include "shared.hpp"
#include <vector>
#include <random>

using namespace std;

class Character;
class InManager;
class GameMap;
//class MapSpec;

class Game{
vector<GameMap*> m_pMap;
InManager* m_pInput;
Character* m_pPlayer;
RNG* m_pGenerator;
MapSpec m_Spec;
string m_LogString;
unsigned short m_Level;
unsigned short m_TotalLevels;
unsigned short m_NumEnemies;
unsigned short m_EnemyGrowth;
unsigned short m_NumItems;
unsigned short m_ItemGrowth;
unsigned short m_ItemStrength;
unsigned short m_ItemStrengthGrowth;
float m_PoisonPercent;
float m_PoisonPercentGrowthFactor;
bool m_IsPlaying;
bool m_IsGameOver;
void DisplayTitle();
void DisplayStory();
void MakeSpace();
void MainLoop();
void NextLevel();
void ResetWorld();
void DestroyWorld();
void CreateWorld();
void PushLog(const string&);
void FlushLog();
bool GameOver(bool = false);
public:
  Game(unsigned short levels = 1, unsigned short enemies = 10, unsigned short enemyGrowth = 2, unsigned short items = 10, unsigned short itemGrowth = 2, unsigned short itemStrength = 3, unsigned short itemStrengthGrowth = 1, float poisonPercent = 0.5f, float poisonPercentGrowth = 1.1f);
  ~Game();
  void Start();
  void QuitMacro();
  void RedoMacro();
  void HelpMacro();

};

#endif
