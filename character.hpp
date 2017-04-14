#ifndef CHARACTER_H
#define CHARACTER_H
#include "entity.hpp"
#include "functional"

class GameMap;
class Decider;

class Character : public Entity {
public:
  Character(const Vec2i &, GameMap *, bool = false, const unsigned int & = 10,
            const unsigned short & = 5, const unsigned short & = 3,
            const unsigned char & = 1);
  ~Character();
  virtual string Move(const Vec2i &, bool = true);
  virtual string Attack(const Vec2i &, bool = false);
  virtual string Use(const Vec2i &, bool = true);
  virtual string Look(const Vec2i &, bool = false);
  virtual string RedoAction(bool = false);
  virtual unsigned int Heal(const unsigned int &);
  //Since Character is meant to be a base class, this function should only be called by derived classes that are also players. Returns "you". Derived classes should return their class name (like "orc")
  virtual string GetName()const;

  //Since Character is meant to be a base class, this function should only be called by derived classes that are players. Returns '@' (player symbol). Derived classes should return their map symbol (like 'o' for orc)
  virtual MapType GetType()const;
  //Override in derived class to create unique behavior between races/classes.
  virtual string TakeDamage(const int&);
  virtual string GetUsed(Character &);
  virtual string GetDescription();
  virtual string Decide();
  virtual void ResetActions();
  virtual int GetActions()const;
protected:
  // Don't think there's enough difference between playable and non playable characters to justify making another class. Input is already handled by InManager.
  bool m_IsPlayer;
  Decider * m_pDecider;
  // Stats
  unsigned int m_MaxHealth;
  unsigned int m_Health;
  unsigned short m_Attack;
  unsigned short m_Defense;
  unsigned char m_MaxActions;
  unsigned char m_Actions;
  unsigned char m_ExpPercent;

  string m_Name;

  //Keeping these protected until I can specify nouns as well as directions
  virtual string AttackTarget(Entity &);
  string UseItem(Entity &);

  // Used for re-performing actions
  function<string(Character&, const Vec2i&, bool)> m_LastAction;
  Vec2i m_LastDirection;

  void SetLastAction(const function<string(Character&, const Vec2i&, bool)> &, const Vec2i &);
};

#endif
