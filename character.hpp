#ifndef CHARACTER_H
#define CHARACTER_H
#include "entity.hpp"

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
  virtual string Look(const Vec2i &, bool = false)const;
  //Only reached if character is player.
  string GetName(bool = false, bool = false)const;

  MapType GetType()const;
  virtual string GetAttacked(const int&, Character &);
  virtual string GetUsed(Character &);
  virtual string GetDescription() const;

protected:
  GameMap *m_pMap;
  bool m_IsPlayer;
  // Stats
  unsigned int m_MaxHealth;
  unsigned int m_Health;
  unsigned short m_Attack;
  unsigned short m_Defense;
  unsigned char m_MaxActions;
  unsigned char m_Actions;
  unsigned char m_ExpPercent;
  virtual void ApplyEffects(string &base, bool, bool)const;
};

#endif
