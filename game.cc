#include "game.hpp"
#include "inmanager.hpp"
#include "gamemap.hpp"
#include "character.hpp"
#include "pig.hpp"
#include "useful.hpp"
#include "healthpotion.hpp"
#include "poisonpotion.hpp"
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
  cout << "GAME";
  m_pInput = new InManager();
  m_pGenerator = new RNG(*m_Spec.GetSeed());

  m_pMap.push_back(new GameMap(m_Spec, m_pGenerator));

  int numPigs = 10;
  int numHP = 5;
  int numPP = 5;
  cout << "RESET";
  m_pMap[0]->ResetRaffle();
  cout << "PULL";
  vector<Vec2i> places = m_pMap[0]->RafflePull(numPigs + numHP + numPP + 2, 10, 5);
  Vec2i first, second;
  cout << "POP";
  PopFurthestPoints(places, first, second);
  m_pMap[0]->SetStartLocation(first);
  m_pMap[0]->SetEndLocation(second);
  m_pPlayer = new Pig(first, m_pMap[0], true);

  if(!m_pMap[0]->TryAddPlayer(m_pPlayer)){
    cout << "Something went wrong with adding character to map";
  }

  for(int i = 0; i < numPigs; i++){
    m_pMap[0]->TryAddEntity(new Pig(*places.begin(), m_pMap[0], false));
    places.erase(places.begin());
  }
  for(int i = 0; i < numHP; i++){
    m_pMap[0]->TryAddEntity(new HealthPotion(3, *places.begin(), m_pMap[0]));
    places.erase(places.begin());
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
    while(m_IsPlaying && m_pPlayer->GetActions() > 0){
      cout << m_pMap[0]->ToString();
      if(getline(cin, input)){
        cout << m_pInput->ParseInput(input, *m_pPlayer, *this);
        m_pMap[0]->UpdateDistanceMap();
      }
      else{
        m_IsPlaying = false;
      }
      if(m_pPlayer->IsDead()){
        m_IsPlaying = false;
      }
    }
    m_pPlayer->ResetActions();
    cout << m_pMap[0]->Update();
    if(m_pPlayer->IsDead()){
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
