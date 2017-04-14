#ifndef DECIDER_H
#define DECIDER_H
#include <string>
class Character;
class GameMap;
//TODO: Follow player when close enough. Attack when closer enough
class Decider{
public:
  Decider();
  ~Decider();
  virtual std::string Decide(Character&, GameMap&, const unsigned int &);
};

#endif
