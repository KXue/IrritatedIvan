#include "character.hpp"
#include "gamemap.hpp"
#include <sstream>

Character::Character(const Vec2i &position, GameMap *map,
                     bool isPlayer,
                     const unsigned int &maxHealth,
                     const unsigned short &attack,
                     const unsigned short &defense,
                     const unsigned char &maxActions) : Entity(position), m_pMap(map), m_IsPlayer(isPlayer), m_MaxHealth(maxHealth), m_Health(maxHealth), m_Attack(attack), m_Defense(defense), m_MaxActions(maxActions), m_Actions(maxActions), m_ExpPercent(0) {}

Character::~Character() { m_pMap = nullptr; }

string Character::Move(const Vec2i &direction, bool quiet) {
  Vec2i newPosition = m_Position + direction;
  stringstream ss;
  ss << GetName(true) << " moves to " << newPosition.GetX() << ", "<< newPosition.GetY() << ".";
  if (m_pMap->GetTileAt(newPosition) == MapType::floor) {
    m_Position = newPosition;
    SetLastAction(&Character::Move, direction);
    m_Actions--;
  }else{
    ss << " But something got in the way!";
  }
  ss << endl;
  if(quiet){
    return "";
  }
  return ss.str();
}
string Character::Attack(const Vec2i &direction, bool quiet) {
  Entity *target = nullptr;
  Vec2i newPosition = m_Position + direction;
  stringstream ss;
  ss << GetName(true) << " is thinking about attacking something at " << newPosition.GetX() << ", " << newPosition.GetY() << ".";

  if(m_pMap->TryGetEntityAt(newPosition, target)){
    ss << target->GetAttacked(m_Attack, *this);
    SetLastAction(&Character::Attack, direction);
    m_Actions--;
  }
  else{
    ss << "But there's nothing there so " << GetName() << " decides not to attack.";
  }

  ss << endl;

  if(quiet){
    return "";
  }
  return ss.str();
}
string Character::Use(const Vec2i &direction, bool quiet) {
  Entity *target = nullptr;
  Vec2i newPosition = m_Position + direction;
  stringstream ss;
  ss << GetName() << " is thinking about using an item at " << newPosition.GetX() << ", " << newPosition.GetY() << ".";

  if(m_pMap->TryGetEntityAt(newPosition, target)){
    ss << target->GetUsed(*this);
    SetLastAction(&Character::Use, direction);
  }
  else{
    ss << "But there's nothing there so " << GetName() << " decides not to do anything.";
  }

  ss << endl;

  if(quiet){
    return "";
  }
  return ss.str();
}

string Character::RedoAction(bool quiet){
  string retVal;
  if(m_LastAction){
    retVal = m_LastAction(*this, m_LastDirection, quiet);
  }else{
    retVal = "This character hasn't done anything yet.\n";
  }
  return retVal;
}

string Character::Look(const Vec2i & direction, bool quiet){
  Entity *target = nullptr;
  Vec2i newPosition = m_Position + direction;
  stringstream ss;
  ss << GetName() << " looks at " << newPosition.GetX() << ", " << newPosition.GetY() << ".";
  if(m_pMap->TryGetEntityAt(newPosition, target)){
    ss << target->GetDescription();
    SetLastAction(&Character::Look, direction);
  }
  else{
    ss << " But there's nothing there.";
  }
  ss << endl;
  return ss.str();
}
string Character::GetName(bool start, bool possessive)const{
  string retVal = "You";
  ApplyEffects(retVal, start, possessive);
  return retVal;
}
void Character::ApplyEffects(string &base, bool start, bool possessive)const{
  if(possessive){
    base += "'re";
  }
  if(start){
    base[0] = tolower(base[0]);
  }
}

MapType Character::GetType()const{
  return MapType::player;
}
string Character::GetAttacked(const int &attackStrength, Character &attacker){
  unsigned int damageTaken = (attackStrength * (float)(10 - m_Defense) * 0.1);

  stringstream ss;
  ss << GetName(true) << " takes " << damageTaken << "damage! ";

  m_Health -= damageTaken;

  if(m_Health <= 0){
    ss << GetName(true) << " died!";
  }else if(m_Health < (float)m_MaxHealth * 0.25){
    ss << GetName(true) << " looks to be in rough shape!";
  }

  return ss.str();
}
string Character::GetUsed(Character &user){
  stringstream ss;
  ss << GetName(true) << " looks at you confusingly like \"WTF are you doing?\" You just wasted an action.";
  return ss.str();
}
string Character::GetDescription() const{
  return "This is some kind of character. Not sure what yet.";
}
void Character::SetLastAction(const function<string(Character&, Vec2i, bool)> &action, const Vec2i &direction){
  m_LastAction = action;
  m_LastDirection = direction;
}
