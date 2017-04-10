#ifndef INMANAGER_H
#define INMANAGER_H

#include "inputtype.hpp"

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

class Game;
class Character;
class Vec2i;

using namespace std;

class InManager{
  unordered_map<string, string> m_AlternativeMap;
  unordered_map<string, function<string(Character&, Vec2i, bool)>> m_ActionMap;
  unordered_map<string, function<void(Game&)>> m_MacroMap;
  unordered_map<string, Vec2i> m_DirectionMap;
  unordered_map<string, InputType> m_TypeMap;
  unordered_map<string, string> m_DescriptionMap;
  vector<string> m_ValidTerms; // only for ToString :(
  string m_LastAction;
  function<void(Game&)>m_QuitFunction;
  bool InputValidity(const string &, string &, InputType &);


public:
  InManager(const string & = "dictionary.json");
  ~InManager();
  string ParseInput(const string&, Character&, Game&);
  void AddFunction(const string&, const function<string(Character&, const Vec2i&, bool)> &);
  void AddMacro(const string&, const function<void(Game&)>&);
  string ToString();
};

#endif
