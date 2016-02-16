#ifndef Parser_h
#define Parser_h
#include "Arduino.h"
#include "Command.h"

class Parser {
  public:
    Parser();
    ~Parser();
    Command* parse(byte* data, uint8_t size);
  private:
    uint16_t searchStart(uint16_t from);
    uint16_t searchEnd(uint16_t from);

    byte* buffer;
    uint8_t bufferLength;
    static const int8_t NOT_FIND = - 1;
};

#endif
