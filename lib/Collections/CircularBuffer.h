#ifndef CircularBuffer_h
#define CircularBuffer_h
#include "Arduino.h"

template<typename T>
class CircularBuffer {
  public:
    CircularBuffer(uint16_t capacity);
    ~CircularBuffer();

    void put(T item);
    T set(uint16_t index, T item);
    T get(uint16_t index);
    T remove(uint16_t index);
    void clear();

    uint16_t size();
    bool isEmpty();

    void print();//@Deprecated
  private:
    uint16_t getAbsoluteIndex(uint16_t index);
    uint16_t getRelativeIndex(uint16_t index);
    bool rangeCheck(uint16_t index);

    uint16_t start;
    uint16_t end;
    uint16_t capacity;
    uint16_t valuesSize;
    T* values;
};

#endif
