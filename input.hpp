#ifndef INPUT_H
#define INPUT_H

#include "inputtype.hpp"

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

class Game;
class Character;
class Vec2i;

using namespace std;

class Input{
  unordered_map<string, string> m_AlternativeMap;
  unordered_map<string, function<string(Character&, Vec2i, bool)>> m_ActionMap;
  unordered_map<string, Vec2i> m_DirectionMap;
  unordered_map<string, InputType> m_TypeMap;
  unordered_map<string, string> m_DescriptionMap;
  vector<string> m_ValidTerms; // only for ToString :(
  string m_LastAction;
  function<void(Game&)>m_QuitFunction;
  bool InputValidity(const string &, string &, InputType &);


public:
  Input(const string & = "dictionary.json");
  ~Input();
  void ParseInput(const string&, Character&, Game&);
  void AddFunction(const string&, const function<string(Character&, const Vec2i&, bool)> &);
  void AddQuitFunction(const function<void(Game&)>&);
  string ToString();
};

#endif
