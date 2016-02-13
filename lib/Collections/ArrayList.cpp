#include "ArrayList.h"
#include "MemoryFree.h"//TODO:@Deprecated

template< typename T >
ArrayList<T>::ArrayList() {
  values = new T[DEFAULT_CAPACITY];
  capacity = DEFAULT_CAPACITY;
  valuesSize = 0;
}

template< typename T >
ArrayList<T>::ArrayList(uint8_t initialCapacity) {
  values = new T[initialCapacity];
  capacity = initialCapacity;
  valuesSize = 0;
}

template< typename T >
ArrayList<T>::~ArrayList() {
  delete[] values;
}

template< typename T >
void ArrayList<T>::add(T item) {
  ensureCapacity(valuesSize + 1);
  values[valuesSize++] = item;
}

template< typename T >
void ArrayList<T>::add(uint16_t index, T item) {
  if (rangeCheck(index == valuesSize ? index - 1 : index)) {
    ensureCapacity(valuesSize + 1);
    memmove(&values[index + 1], &values[index], (valuesSize - index) * sizeof(T));
    values[index] = item;
    valuesSize++;
  }
}

template< typename T >
T ArrayList<T>::set(uint16_t index, T item) {
  if (rangeCheck(index)) {
    T oldValue = values[index];
    values[index] = item;
    return oldValue;
  } else return (T) NULL;
}

template< typename T >
T ArrayList<T>::get(uint16_t index) {
  if (rangeCheck(index)) {
    return values[index];
  } else return (T) NULL;
}

template< typename T >
T ArrayList<T>::remove(uint16_t index) {//TODO: check
  if (rangeCheck(index)) {
    T removed = values[index];
    if (index < valuesSize - 1) {
      memmove(&values[index], &values[index + 1], (valuesSize - index - 1) * sizeof(T));//TODO: check
    }
    valuesSize--;
    return removed;
  } else return (T) NULL;
}

template< typename T >
void ArrayList<T>::clear() {
  valuesSize = 0;
}

template< typename T >
uint16_t ArrayList<T>::size() {
  return valuesSize;
}

template< typename T >
bool ArrayList<T>::isEmpty() {
  return valuesSize == 0;
}

template< typename T >
void ArrayList<T>::trimToSize() {
  if (capacity > valuesSize) {
    T* newValues = new T[valuesSize];
    memcpy(newValues, values, valuesSize * sizeof(T));
    delete[] values;
    values = newValues;
    capacity = valuesSize;
  }
}

template< typename T >
bool ArrayList<T>::rangeCheck(uint16_t index) {
  if (index < size()) {
    return true;
  } else {
    Serial.print(F("IndexOutOfBoundsException: Index: ")); Serial.print(index); Serial.print(F(", Size: ")); Serial.println(valuesSize);
    return false;
  }
}

template< typename T >
void ArrayList<T>::ensureCapacity(uint16_t minCapacity) {
  if (capacity < minCapacity) grow(minCapacity);
}

template< typename T >
void ArrayList<T>::grow(uint16_t minCapacity) {
  uint16_t newCapacity = capacity + (capacity >> 1);
  if (newCapacity < minCapacity) newCapacity = minCapacity;
  T* newValues = new T[newCapacity];
  memcpy(newValues, values, capacity * sizeof(T));
  delete[] values;
  values = newValues;
  capacity = newCapacity;
}

//@Deprecated
template< typename T >
void ArrayList<T>::print() {
  Serial.println(F("-------------------"));
  Serial.print(F("RAM: ")); Serial.println(freeMemory());
  Serial.print(F("Capacity: ")); Serial.print(capacity);
  Serial.print(F(" | size = ")); Serial.println(valuesSize);
  for (uint16_t i = 0; i < valuesSize; i++) {
    Serial.print(F("@")); Serial.print((uint16_t) &values[i]); Serial.print(F(" = ")); Serial.print(values[i]); Serial.print(F(", "));
  }
  Serial.println(F("\n-------------------"));
}
