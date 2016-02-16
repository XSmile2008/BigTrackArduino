#ifndef Argument_h
#define Argument_h
#include "Arduino.h"

class Argument {
  public:
    Argument(byte key, byte size, void* value);
    ~Argument();
    byte getKey();
    byte getSize();
    void* getValue();
    void print();

    static const uint8_t KEY = 0;
    static const uint8_t SIZE = 1;
    static const uint8_t OFFSET = 2;
  private:
    byte key;
	  byte size;
	  void* value;
};

#endif
