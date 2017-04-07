#ifndef CHARACTER_H
#define CHARACTER_H
#include "entity.hpp"
#include "functional"

class GameMap;

class Character : public Entity {
public:
  Character(const Vec2i &, GameMap *, bool = false, const unsigned int & = 10,
            const unsigned short & = 5, const unsigned short & = 3,
            const unsigned char & = 1);
  ~Character();
  virtual string Move(const Vec2i &, bool = true);
  virtual string Attack(const Vec2i &, bool = true);
  virtual string Use(const Vec2i &, bool = true);
  virtual string Look(const Vec2i &, bool = false);
  virtual string RedoAction(bool = false);
  //Only reached if character is player.
  string GetName(bool = false, bool = false)const;

  MapType GetType()const;
  virtual string GetAttacked(const int&, Character &);
  virtual string GetUsed(Character &);
  virtual string GetDescription() const;

protected:
  GameMap *m_pMap;
  // Don't think there's enough difference between playable and non playable characters
  bool m_IsPlayer;
  // Stats
  unsigned int m_MaxHealth;
  unsigned int m_Health;
  unsigned short m_Attack;
  unsigned short m_Defense;
  unsigned char m_MaxActions;
  unsigned char m_Actions;
  unsigned char m_ExpPercent;

  // Used for re-performing actions
  function<string(Character&, Vec2i, bool)> m_LastAction;
  Vec2i m_LastDirection;

  void SetLastAction(const function<string(Character&, Vec2i, bool)> &, const Vec2i &);
  virtual void ApplyEffects(string &base, bool, bool)const;
};

#endif
