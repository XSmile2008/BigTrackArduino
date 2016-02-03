#ifndef Argument_h
#define Argument_h
#include "Arduino.h"

class Argument {
  public:
    Argument(byte bytes[], int pos);
    ~Argument();
    byte getKey();
    byte getSize();
    void* getValue();
    void toString();
  private:
    byte key;
	  byte size;
	  void* value;
};

#endif
