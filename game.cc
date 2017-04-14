#include "game.hpp"
#include "inmanager.hpp"
#include "gamemap.hpp"
#include "character.hpp"
#include "pig.hpp"
#include "useful.hpp"
#include <functional>
#include <fstream>
#include <iostream>
void Game::CreatePlayer(){

}
void Game::DisplayTitle(){

}
void Game::MainLoop(){

}
Game::Game() : m_IsPlaying(true){
  m_pInput = new InManager();
  m_pGenerator = new RNG(*m_Spec.GetSeed());

  m_pMap.push_back(new GameMap(m_Spec, m_pGenerator));

  int numPigs = 10;
  int numHP = 5;
  int numPP = 5;

  vector<Vec2i> places = m_pMap[0]->RafflePull(numPigs + numHP + numPP, 10, 5);

  m_pMap[0]->ResetRaffle();
  Vec2i playerPosition = m_pMap[0]->RafflePull(1, 10, 5)[0];
  m_pPlayer = new Pig(playerPosition, m_pMap[0], true);

  if(!m_pMap[0]->TryAddPlayer(m_pPlayer)){
    cout << "Something went wrong with adding character to map";
  }

  m_pInput->AddFunction("move", &Character::Move);
  m_pInput->AddFunction("attack", &Character::Attack);
  m_pInput->AddFunction("use", &Character::Use);
  m_pInput->AddFunction("look", &Character::Look);

  m_pInput->AddMacro("quit", &Game::QuitMacro);
  m_pInput->AddMacro("again", &Game::RedoMacro);
  m_pInput->AddMacro("help", &Game::HelpMacro);
}
Game::~Game(){
  delete m_pInput;
  delete m_pGenerator;
  delete m_pPlayer;
  for(int i = 0; i < m_pMap.size(); i++){
    delete m_pMap[i];
  }
}
void Game::Start(){
  m_IsPlaying = true;
  string input;
  while(m_IsPlaying){
    cout << m_pMap[0]->ToString();
    if(getline(cin, input)){
      cout << m_pInput->ParseInput(input, *m_pPlayer, *this);
      m_pMap[0]->UpdateDistanceMap();
    }
    else{
      m_IsPlaying = false;
    }
  }
}
void Game::QuitMacro(){
  m_IsPlaying = false;
}
void Game::RedoMacro(){
  cout << m_pPlayer->RedoAction(false);
}
void Game::HelpMacro(){
  cout << m_pInput->ToString() << endl;
  cout << "Press enter to continue: ";
  string temp;
  getline(cin, temp);
}
