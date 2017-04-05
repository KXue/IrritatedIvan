#ifndef INPUTTYPES_H
#define INPUTTYPES_H

enum class InputType : char { macro = 0, verb, direction};
static const char* INPUTTYPESTRING[] = {
  "macro",
  "verb",
  "direction"
};

#endif
