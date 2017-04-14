#include "decider.hpp"
#include "character.hpp"
#include "gamemap.hpp"

Decider::Decider(){}

Decider::~Decider(){}

std::string Decider::Decide(Character &character, GameMap &map, const unsigned int &hostileDistance){
  std::string retVal = "";
  while(character.GetActions() > 0){
    unsigned int stepsToPlayer = map.GetDistanceToPlayer(character.GetPosition());
    if(stepsToPlayer <= 1){
      retVal += character.Attack(map.DirectionToPlayer(character.GetPosition()), false);
    }
    else if(stepsToPlayer <= hostileDistance){
      retVal += character.Move(map.DirectionToPlayer(character.GetPosition()), true);
    }
    else{
      retVal += character.Move(map.RandomValidDirection(character.GetPosition()), true);
    }
  }
  character.ResetActions();
  return retVal;
}
