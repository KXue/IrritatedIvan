#include "character.hpp"
#include "gamemap.hpp"
#include "useful.hpp"
#include "decider.hpp"
#include <iostream>
#include <sstream>
Character::Character(const Vec2i &position, GameMap *map,
                     bool isPlayer,
                     const unsigned int &maxHealth,
                     const unsigned short &attack,
                     const unsigned short &defense,
                     const unsigned char &maxActions) : Entity(position, map), m_IsPlayer(isPlayer), m_pDecider(new Decider()), m_MaxHealth(maxHealth), m_Health(maxHealth), m_Attack(attack), m_Defense(defense), m_MaxActions(maxActions), m_Actions(maxActions), m_ExpPercent(0) {}

Character::~Character() {
  if(m_pDecider){
    delete m_pDecider;
    m_pDecider = nullptr;
  }
}
string Character::AttackTarget(Entity &target){
  return target.TakeDamage(m_Attack);
}
string Character::UseItem(Entity &target){
  return target.GetUsed(*this);
}
string Character::Move(const Vec2i &direction, bool quiet) {
  Vec2i newPosition = m_Position + direction;
  stringstream ss;
  if (m_pMap->GetTileAt(newPosition) == MapType::floor) {
    string move = m_IsPlayer? "move" : "moves";
    ss << Capitalize(GetName()) << " " << move << " one step.";
    m_Position = newPosition;
    SetLastAction(&Character::Move, direction);
    --m_Actions;
  }else{
    ss << "Something is in the way!";
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
  if(m_pMap->TryGetEntityAt(newPosition, target)){
    string attackString = m_IsPlayer? "attack" : "attacks";
    ss << Capitalize(GetName()) << " " << attackString << " " << target->GetName() << ". ";
    ss << AttackTarget(*target);
    SetLastAction(&Character::Attack, direction);
    --m_Actions;
  }
  else{
    string decideString = m_IsPlayer? "decide" : "decides";
    ss << "There's nothing there so " << GetName() << " " << decideString << " not to attack. ";
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

  if(m_pMap->TryGetEntityAt(newPosition, target)){
    string useString = m_IsPlayer? "use" : "uses";
    ss << Capitalize(GetName()) << " " << useString << " " << target->GetName() << ". ";
    ss << UseItem(*target);
    SetLastAction(&Character::Use, direction);
    --m_Actions;
  }
  else{
    string decideString = m_IsPlayer? "decide" : "decides";
    ss << "There's nothing there so " << GetName() << " " << decideString <<" not to do anything.";
  }

  ss << endl;

  if(quiet){
    return "";
  }
  return ss.str();
}

string Character::Look(const Vec2i & direction, bool quiet){
  Entity *target = nullptr;
  Vec2i newPosition = m_Position + direction;
  stringstream ss;
  if(m_pMap->TryGetEntityAt(newPosition, target)){
    string lookString = m_IsPlayer? "look" : "looks";
    ss << Capitalize(GetName()) << " " << lookString << " at " << target->GetName() << ". ";
    ss << target->GetDescription();
    SetLastAction(&Character::Look, direction);
    --m_Actions;
  }
  else{
    ss << "There's nothing there.";
  }
  ss << endl;
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

string Character::GetName()const{
  return "you";
}
MapType Character::GetType()const{
  return MapType::player;
}
string Character::TakeDamage(const int &attackStrength){
  unsigned int damageTaken = (attackStrength * (float)(10 - m_Defense) * 0.1);

  stringstream ss;
  string takeString = m_IsPlayer? "take" : "takes";
  ss << Capitalize(GetName()) << " " << takeString << " " << damageTaken << " damage! ";

  if(damageTaken >= m_Health){
    damageTaken = m_Health;
  }
  m_Health -= damageTaken;

  if(m_Health <= 0){
    ss << Capitalize(GetName()) << " died! ";
    m_pMap->TryRemoveEntityAt(GetPosition());
  }else if(m_Health < (float)m_MaxHealth * 0.25){
    string lookString = m_IsPlayer? "look" : "looks";
    ss << Capitalize(GetName()) << " " << lookString << " to be in rough shape! ";
  }
  return ss.str();
}
string Character::GetUsed(Character &user){
  stringstream ss;
  ss << Capitalize(GetName()) << " looks at you confusingly like \"WTF are you doing?\" You just wasted an action.";
  return ss.str();
}
string Character::GetDescription(){
  return "This is some kind of character. Not sure what yet.";
}
void Character::SetLastAction(const function<string(Character&, const Vec2i&, bool)> &action, const Vec2i &direction){
  m_LastAction = action;
  m_LastDirection = direction;
}
string Character::Heal(const unsigned int &amount){
  unsigned int retVal = m_MaxHealth - m_Health;
  if(amount < retVal){
    retVal = amount;
  }
  m_Health += retVal;
  stringstream ss;
  ss << Capitalize(GetName()) << " ";
  string healString = m_IsPlayer ? "heal" : "heals";
  ss << healString << " for " << retVal << " HP. " << endl;
  return ss.str();
}
void Character::ResetActions(){
  m_Actions = m_MaxActions;
}
int Character::GetActions()const{
  return m_Actions;
}
string Character::Decide(){
  return m_pDecider->Decide(*this, *m_pMap, 3);
}
bool Character::IsDead()const{
  return m_Health == 0;
}
string Character::ToString()const{
  stringstream ss;
  ss << Capitalize(GetName()) << " Health: " << m_Health << "/" << m_MaxHealth << " Attack: " << m_Attack << " Defense: " << m_Defense << endl;
  ss << "Actions: " << m_Actions << "/" << m_MaxActions << endl;
  return ss.str();
}
void Character::ChangeMap(GameMap* map){
  m_pMap = map;
}
void Character::Reposition(const Vec2i &newPosition){
  m_Position = newPosition;
}
