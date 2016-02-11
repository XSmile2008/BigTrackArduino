#include "ArrayList.h"

template< typename T >
int ArrayList<T>::size() {
  return arraySize;
}

template< typename T >
bool ArrayList<T>::isEmpty() {
  return arraySize == 0;
}

template< typename T >
void ArrayList<T>::add(T item) {

}

template< typename T >
T ArrayList<T>::set(int index, T item) {

}

template< typename T >
void ArrayList<T>::rangeCheckForAdd(int index) {
  if (index < arraySize) return true;
  else false;
}
