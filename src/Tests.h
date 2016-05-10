#ifndef Tests_h
#define Tests_h
#include "Arduino.h"
#include "MemoryFree.h"
#include "Parser.h"
#include "ArrayList.h"
#include "CircularBuffer.h"

class Tests {
  public:
    static void lifeTest();
    static void listTest();
    static void circularBufferTest();
    static void commandTest();
    static void printBytes(byte bytes[], int size);
};

#endif
