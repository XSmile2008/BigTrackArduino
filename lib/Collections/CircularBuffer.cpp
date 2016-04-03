#include "CircularBuffer.h"

template< typename T >
CircularBuffer<T>::CircularBuffer(uint16_t capacity) {
  this->values = new T[capacity];
  this->capacity = capacity;
  this->valuesSize = 0;
  this->start = 0;
  this->end = 0;
}

template< typename T >
CircularBuffer<T>::~CircularBuffer() {
  delete[] values;
}

template< typename T >
void CircularBuffer<T>::put(T item) {
  if (valuesSize != 0) {
    end++;
    if (end == capacity) end = 0;
    if (start == end) {
      start++;
      if (start == capacity) start = 0;
    } else {
      valuesSize++;
    }
  } else {
    valuesSize++;
  }
  values[end] = item;
}

template< typename T >
T CircularBuffer<T>::set(uint16_t index, T item) {
  if (rangeCheck(index)) {
    T oldValue = values[getAbsoluteIndex(index)];
    values[getAbsoluteIndex(index)] = item;
    return oldValue;
  } else return (T) NULL;
}

template< typename T >
T CircularBuffer<T>::get(uint16_t index) {
  if (rangeCheck(index)) {
    return values[getAbsoluteIndex(index)];
  } else return (T) NULL;
}

template< typename T >
T CircularBuffer<T>::remove(uint16_t index) {//TODO: check
  if (rangeCheck(index)) {
    uint16_t absoluteIndex = getAbsoluteIndex(index);
    T removed = values[absoluteIndex];
    if (start < end) {
      if (absoluteIndex < valuesSize - 1) { //TODO: this check is important to avoid move wrong memory
        memmove(&values[absoluteIndex], &values[absoluteIndex + 1], (valuesSize - absoluteIndex - 1) * sizeof(T));//TODO: check
      }
    } else if (start > end) {

    }
    valuesSize--;
    return removed;
  } else return (T) NULL;
}

template< typename T >
void CircularBuffer<T>::clear() {
  valuesSize = 0;
  start = 0;
  end = 0;
}

template< typename T >
uint16_t CircularBuffer<T>::size() {
  return valuesSize;
}

template< typename T >
bool CircularBuffer<T>::isEmpty() {
  return valuesSize == 0;
}

template< typename T >
uint16_t CircularBuffer<T>::getAbsoluteIndex(uint16_t index) {
  uint16_t absoluteIndex = start + index;
  return (absoluteIndex >= capacity) ? absoluteIndex - capacity : absoluteIndex;
}

template< typename T >
bool CircularBuffer<T>::rangeCheck(uint16_t index) {
  if (index < valuesSize) {
    return true;
  } else {
    Serial.print(F("IndexOutOfBoundsException: Index: ")); Serial.print(index); Serial.print(F(", Size: ")); Serial.println(size());
    return false;
  }
}

//@Deprecated
template< typename T >
void CircularBuffer<T>::print(){
  Serial.println(F("Root data: "));
  for (int i = 0; i < capacity; i++) {
    Serial.print(values[i]); Serial.print(F(", "));
  } Serial.println();
  Serial.print(F("start = ")); Serial.print(start); Serial.print(F(", end = ")); Serial.println(end);
}
