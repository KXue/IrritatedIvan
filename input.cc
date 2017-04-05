#include "input.hpp"
#include "vec2i.hpp"
#include "json.hpp"
#include <fstream>
#include <sstream>

using json = nlohmann::json;

Input::Input(const string &fileLocation){
  InputType type;
  string term;
  string alternative;
  string description;
  fstream specFile(fileLocation);
  json j;
  specFile >> j;
  unordered_map<string, InputType> inputEnumMap;
  for(int i = 0; i <= (int)InputType::direction; i++){
    inputEnumMap[INPUTTYPESTRING[i]] = static_cast<InputType>(i);
  }
  for(auto& element : j){
    type = inputEnumMap[element["type"].get<string>()];
    term = element["term"].get<string>();
    description = element["description"].get<string>();
    m_ValidTerms.push_back(term);
    m_TypeMap[term] = type;
    m_DescriptionMap[term] = description;
    if(type == InputType::direction){
      m_DirectionMap[term] = Vec2i(element["vector"][0].get<int>(), element["vector"][1].get<int>());
    }
    for(int i = 0; i < element["alternatives"].size(); i++){
      alternative = element["alternatives"][i].get<string>();
      m_AlternativeMap[alternative] = term;
    }
  }
}

Input::~Input(){}
void Input::ParseInput(const string &input, Character &character, Game &game){
  stringstream ss(input);
  string substring;
  string term;
  InputType type;
  function<string(Character&, const Vec2i&, bool)> action;
  bool valid = true;

  if(ss >> substring){
    valid = InputValidity(substring, term, type);
  }
  else{
    cout << "No input found";
    valid = false;
  }

  if(valid){
    cout << term << " " << INPUTTYPESTRING[(int)type] << endl;
  }

  if(valid && type != InputType::macro && type != InputType::verb){
    cout << "Invalid input: " << substring << ". The first word should either be a verb or macro!" << endl;
    valid = false;
  }

  if(valid && type == InputType::verb){
    action = m_ActionMap[term];
  }
  else if(valid && type == InputType::macro){
    if(term == "again"){
      if(m_LastAction.empty()){
        cout << "No previous action found." << endl;
      }
      else {
        ParseInput(m_LastAction, character, game);
      }
    }
    else if(term == "help"){
      cout << ToString() << endl;
      cout << "Press enter to continue: ";
      string temp;
      getline(cin, temp);
    }
    else{
      m_QuitFunction(game);
    }
    valid = false;

  }
  else if(valid){
    cout << "Uhh something went wrong here. " << term << " somehow doesn't work" << endl;
    valid = false;
  }

  if(valid && ss >> substring){
    valid = InputValidity(substring, term, type);
  }
  else if(valid){
    cout << "Action requires direction: " << substring << endl;
    valid = false;
  }

  if(valid){
    cout << term << " " << INPUTTYPESTRING[(int)type] << endl;
  }

  if(valid && type == InputType::direction){
    cout << action(character, m_DirectionMap[term], false);
  }


  if(valid){
    m_LastAction = input;
  }
}

bool Input::InputValidity(const string &input, string &term, InputType &type){
  bool valid = true;

  if(m_AlternativeMap.find(input) != m_AlternativeMap.end()){
    term = m_AlternativeMap[input];
  }
  else{
    term = input;
  }

  if(m_TypeMap.find(term) != m_TypeMap.end()){
    type = m_TypeMap[term];
  }
  else{
    cout << "Invalid input: " << input << " isn't a recognized term!" << endl;
    valid = false;
  }

  return valid;
}

void Input::AddFunction(const string &verb, const function<string(Character &, const Vec2i &, bool)> &action){
  if(m_TypeMap.find(verb) != m_TypeMap.end() && m_TypeMap[verb]==InputType::verb){
    m_ActionMap[verb] = action;
  }
}

string Input::ToString(){
  unordered_map<string, vector<string>> reverseAlternativeMap;
  for(auto &alternative : m_AlternativeMap){
    reverseAlternativeMap[alternative.second].push_back(alternative.first);
  }

  stringstream ss;

  ss << endl << endl << "__________HELP__________" << endl << endl;

  for(auto &term : m_ValidTerms){
    ss << term << "(" << INPUTTYPESTRING[(int)m_TypeMap[term]] << ")" ":" << endl;
    ss << "Alternatives: ";
    for(auto &alternative : reverseAlternativeMap[term]){
      ss << alternative << " ";
    }
    ss << endl;
    ss << "Description: ";
    ss << m_DescriptionMap[term] << endl << endl;
  }
  return ss.str();
}
void Input::AddQuitFunction(const function<void(Game&)> &fcn){
  m_QuitFunction = fcn;
}
