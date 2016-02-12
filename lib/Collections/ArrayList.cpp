#include "ArrayList.h"

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
  ensureCapacity(valuesSize +1);
  values[valuesSize++] = item;
  Serial.print("size = "); Serial.println(size());
}

template< typename T >
void ArrayList<T>::add(int index, T item) {
  ensureCapacity(valuesSize + 1);
  valuesSize++;
  //TODO:
}

template< typename T >
T ArrayList<T>::set(int index, T item) {
  rangeCheck(index);
  T oldValue = values[index];
  values[index] = item;
  return oldValue;
}

template< typename T >
T ArrayList<T>::get(int index) {
  return values[index];
}

template< typename T >
T ArrayList<T>::remove(int index) {//TODO: check
  if (index < valuesSize - 1)
    memcpy(&values[index], &values[index + 1], valuesSize - index);
  valuesSize--;
}

template< typename T >
void ArrayList<T>::clear() {//TODO: check
  delete[] values;
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
void ArrayList<T>::trimToSize() {//TODO: check
  if (capacity > valuesSize) {
    delete [] values[valuesSize + 1];
    capacity = valuesSize;
  }
}

template< typename T >
bool ArrayList<T>::rangeCheck(int index) {
  return (index < size());
}

template< typename T >
void ArrayList<T>::ensureCapacity(int minCapacity) {
  if (capacity < minCapacity) grow(minCapacity);
}

template< typename T >
void ArrayList<T>::grow(int minCapacity) {
  int newCapacity = capacity + (capacity >> 1);
  if (newCapacity < minCapacity) newCapacity = minCapacity;
  Serial.print("new capacity = "); Serial.println(newCapacity);
  T* newValues = new T[newCapacity];
  memcpy(newValues, values, capacity * sizeof(T));
  delete[] values;
  values = newValues;
  capacity = newCapacity;
}
