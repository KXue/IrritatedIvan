#include "game.hpp"
#include "input.hpp"
#include "gamemap.hpp"
#include "character.hpp"
#include "pig.hpp"
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
  m_pInput = new Input();
  m_pInput->AddQuitFunction(&Game::QuitFunction);
  m_pGenerator = new RNG(*m_Spec.GetSeed());

  for(int i = 0; i < 5; i++){
    m_pMap.push_back(new GameMap(m_Spec, m_pGenerator));
  }

  m_pMap[0]->ResetRaffle();
  Vec2i playerPosition = m_pMap[0]->RafflePull(1, 10, 5)[0];
  m_pPlayer = new Pig(playerPosition, m_pMap[0], true);

  if(!m_pMap[0]->TryAddEntity(m_pPlayer)){
    cout << "Something went wrong with adding character to map";
  }

  m_pInput->AddFunction("move", &Character::Move);
  m_pInput->AddFunction("attack", &Character::Attack);
  m_pInput->AddFunction("use", &Character::Use);
  m_pInput->AddFunction("look", &Character::Look);
}
Game::~Game(){
  delete m_pInput;
  delete m_pGenerator;
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
      m_pInput->ParseInput(input, *m_pPlayer, *this);
    }
    else{
      m_IsPlaying = false;
    }
  }
}
void Game::QuitFunction(){
  m_IsPlaying = false;
}
