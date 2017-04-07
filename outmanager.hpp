#ifndef OUTMANAGER_H
#define OUTMANAGER_H

#include <sstream>
#include <iostream>

using namespace std;

class OutManager{
stringstream cache;
public:
  void Push(const string &);
  void Dump();
};

#endif
