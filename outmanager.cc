#include "outmanager.hpp"

void OutManager::Push(const string & input){
  cache << input;
}
void OutManager::Dump(){
  cout << cache.rdbuf();
  cache.str("");
}
