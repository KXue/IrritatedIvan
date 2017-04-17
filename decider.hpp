#ifndef DECIDER_H
#define DECIDER_H
#include <string>
class Character;
class GameMap;
class Decider{
public:
  Decider();
  ~Decider();
  std::string Decide(Character&, GameMap&, const unsigned int &);
};

#endif
