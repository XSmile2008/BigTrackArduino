#include "ArrayList.h"
#include "MemoryFree.h"//TODO:@Deprecated

template< typename T >
ArrayList<T>::ArrayList() {
  values = new T[DEFAULT_CAPACITY];
  capacity = DEFAULT_CAPACITY;
  valuesSize = 0;
}

template< typename T >
ArrayList<T>::ArrayList(int initialCapacity) {
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
void ArrayList<T>::add(int index, T item) {//TODO: complete and test
  if (rangeCheck(index)) {
    ensureCapacity(valuesSize + 1);
    valuesSize++;
    memcpy(&values[index + 1], &values[index], (valuesSize - index) * sizeof(T));
  }
}

template< typename T >
T ArrayList<T>::set(int index, T item) {
  if (rangeCheck(index)) {
    T oldValue = values[index];
    values[index] = item;
    return oldValue;
  } else {
    return item;
  }
}

template< typename T >
T ArrayList<T>::get(int index) {
  if (rangeCheck(index)) {
    return values[index];
  }
}

template< typename T >
T ArrayList<T>::remove(int index) {//TODO: check
  if (rangeCheck(index)) {
    T removed = values[index];
    if (index < valuesSize - 1) {
      memmove(&values[index], &values[index + 1], (valuesSize - index - 1) * sizeof(T));//TODO: check
    }
    valuesSize--;
    return removed;
  }
}

template< typename T >
void ArrayList<T>::clear() {
  valuesSize = 0;
}

template< typename T >
int ArrayList<T>::size() {
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
bool ArrayList<T>::rangeCheck(int index) {
  if (index < size()) {
    return true;
  } else {
    Serial.print(F("IndexOutOfBoundsException: Index: ")); Serial.print(index); Serial.print(F(", Size: ")); Serial.println(valuesSize);
    return false;
  }

}

template< typename T >
void ArrayList<T>::ensureCapacity(int minCapacity) {
  if (capacity < minCapacity) grow(minCapacity);
}

template< typename T >
void ArrayList<T>::grow(int minCapacity) {
  int newCapacity = capacity + (capacity >> 1);
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
  for (int i = 0; i < capacity; i++) {
    Serial.print(F("@")); Serial.print((int) &values[i]); Serial.print(F(" = ")); Serial.print(values[i]); Serial.print(F(", "));
  }
  Serial.println(F("\n-------------------"));
}
