#ifndef GAME_H
#define GAME_H
#include "mapspec.hpp"
#include "shared.hpp"
#include <vector>
#include <random>

using namespace std;

class Character;
class Input;
class GameMap;
//class MapSpec;

class Game{
vector<GameMap*> m_pMap;
Input* m_pInput;
Character* m_pPlayer;
RNG* m_pGenerator;
MapSpec m_Spec;

bool m_IsPlaying;

void CreatePlayer();
void DisplayTitle();
void MainLoop();
public:
  Game();
  ~Game();
  void Start();
  void QuitFunction();
};

#endif
