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
void Game::MakeSpace(){
  for(int i = 0; i < 20; i++){
    cout << endl;
  }
}
void Game::DisplayTitle(){
  fstream fs;
  fs.open("title");
  string titleString;
  string tempString;
  while(getline(fs, tempString)){
    titleString += tempString + '\n';
  }
  fs.close();

  char choiceChar;
  do{
    MakeSpace();
    cout << titleString;
    cout << ": ";
    cin.get(choiceChar);
    choiceChar = tolower(choiceChar);
    cin.clear(); // unset failbit
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // skip bad input
    if(choiceChar == 's'){
      DisplayStory();
    }
  }while(choiceChar != 'b' && choiceChar != 'q');

  m_IsPlaying = choiceChar == 'b';
}
void Game::DisplayStory(){
  MakeSpace();
  fstream fs;
  fs.open("story");
  string tempString;
  while(getline(fs, tempString)){
    cout << tempString << endl;
  }
  fs.close();
  cout << endl << "Press enter to return to main menu: ";
  getline(cin, tempString);
}
void Game::CreateWorld(){
  float changingPoisonPercent = m_PoisonPercent;
  Vec2i begin;
  for(int i = 0; i < m_TotalLevels; i++){
    m_pMap.push_back(new GameMap(m_Spec, m_pGenerator));

    int numPigs = m_NumEnemies + i * m_EnemyGrowth;
    int numHP = (m_NumItems + i * m_ItemGrowth) * (1 - changingPoisonPercent);
    int numPP = (m_NumItems + i * m_ItemGrowth) * changingPoisonPercent;

    changingPoisonPercent *= m_PoisonPercentGrowthFactor;

    m_pMap[i]->ResetRaffle();
    vector<Vec2i> places = m_pMap[i]->RafflePull(numPigs + numHP + numPP + 2, 10, 5);
    Vec2i first, second;
    PopFurthestPoints(places, first, second);
    m_pMap[i]->SetStartLocation(first);
    m_pMap[i]->SetEndLocation(second);

    for(int j = 0; j < numPigs; j++){
      m_pMap[i]->TryAddEntity(new Pig(*places.begin(), m_pMap[i], false));
      places.erase(places.begin());
    }
    for(int j = 0; j < numHP; j++){
      m_pMap[i]->TryAddEntity(new HealthPotion(m_ItemStrength + (i * m_ItemStrengthGrowth), *places.begin(), m_pMap[i]));
      places.erase(places.begin());
    }
    for(int j = 0; j < numPP; j++){
      m_pMap[i]->TryAddEntity(new PoisonPotion(m_ItemStrength + (i * m_ItemStrengthGrowth), *places.begin(), m_pMap[i]));
      places.erase(places.begin());
    }
    if(i == 0){
      begin = first;
    }
  }
  m_pPlayer = new Character(begin, m_pMap[0], true);

  if(!m_pMap[0]->TryAddPlayer(m_pPlayer)){
    cout << "Something went wrong with adding character to map";
  }
}
void Game::DestroyWorld(){
  FlushLog();
  m_Level = 0;
  delete m_pPlayer;
  for(int i = 0; i < m_pMap.size(); i++){
    delete m_pMap[i];
  }
  m_pMap.clear();
}
void Game::ResetWorld(){
  DestroyWorld();
  CreateWorld();
}
void Game::MainLoop(){
  string input;
  while(m_IsPlaying && !m_IsGameOver){
    while(m_IsPlaying && !m_IsGameOver && m_pPlayer->GetActions() > 0){
      MakeSpace();
      cout << m_pMap[m_Level]->ToString();
      cout << m_LogString << endl;
      cout << m_pPlayer->ToString() << endl;
      cout << "Your action (h for help): ";
      if(getline(cin, input)){
        FlushLog();
        PushLog(m_pInput->ParseInput(input, *m_pPlayer, *this));
        m_pMap[m_Level]->UpdateDistanceMap();
      }
      else{
        m_IsPlaying = false;
      }
      if(m_pPlayer->IsDead()){
        m_IsGameOver = true;
      }
      if(m_pPlayer->GetPosition() == m_pMap[m_Level]->GetEndLocation()){
        NextLevel();
      }
    }
    if(m_IsPlaying && !m_IsGameOver){
      m_pPlayer->ResetActions();
      PushLog(m_pMap[m_Level]->Update());
      if(!m_IsGameOver){
        m_IsGameOver = m_pPlayer->IsDead();
      }
    }
    if(m_IsGameOver){
      cout << m_LogString << endl;
    }
  }
}

bool Game::GameOver(bool win){
  cout << "Game Over: You" << (win? " win":" lose") << "!" << endl;
  char choiceChar;
  do{
    cout << "Play again (y/n)?: ";
    cin.get(choiceChar);
    choiceChar = tolower(choiceChar);
    cin.clear(); // unset failbit
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // skip bad input
  }while(choiceChar != 'n' && choiceChar != 'y');
  return choiceChar == 'y';
}
void Game::NextLevel(){
  m_Level++;
  if(m_Level == m_pMap.size()){
    m_IsGameOver = true;
  }
  else{
    m_pMap[m_Level]->TryAddPlayer(m_pMap[m_Level - 1]->TryRemovePlayer());
  }
}
Game::Game(unsigned short levels, unsigned short enemies, unsigned short enemyGrowth, unsigned short items, unsigned short itemGrowth, unsigned short itemStrength, unsigned short itemStrengthGrowth, float poisonPercent, float poisonPercentGrowth) : m_LogString("Log:\n"), m_Level(0), m_TotalLevels(levels), m_NumEnemies(enemies), m_EnemyGrowth(enemyGrowth), m_NumItems(items), m_ItemGrowth(itemGrowth), m_ItemStrength(itemStrength), m_ItemStrengthGrowth(itemStrengthGrowth), m_PoisonPercent(poisonPercent), m_PoisonPercentGrowthFactor(poisonPercentGrowth), m_IsPlaying(true), m_IsGameOver(false){
  m_pInput = new InManager();
  m_pGenerator = new RNG(*m_Spec.GetSeed());
  CreateWorld();
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
  do{
    m_IsGameOver = false;
    DisplayTitle();
    if(m_IsPlaying){
      MainLoop();
      if(m_IsPlaying){
        m_IsPlaying = GameOver(!m_pPlayer->IsDead());
      }
      ResetWorld();
    }
  }while(m_IsPlaying);
}
void Game::QuitMacro(){
  m_IsPlaying = false;
}
void Game::RedoMacro(){
  PushLog(m_pPlayer->RedoAction(false));
}
void Game::HelpMacro(){
  cout << m_pInput->ToString() << endl;
  fstream fs;
  fs.open("legend");
  string tempString;
  while(getline(fs, tempString)){
    cout << tempString << endl;
  }
  fs.close();
  cout << endl << "Press enter to continue: ";
  string temp;
  getline(cin, temp);
}
void Game::PushLog(const string& message){
  m_LogString += message;
}
void Game::FlushLog(){
  m_LogString = "Log:\n";
}
