#ifndef DECIDER_H
#define DECIDER_H
class Character;
class GameMap;
//TODO: Follow player when close enough. Attack when close enough
class Decider{
public:
  virtual void Decide(Character&, GameMap&);
};

#endif
