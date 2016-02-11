#ifndef ArrayList_h
#define ArrayList_h
#include "List.h"

#define DEFAULTCAPACITY_EMPTY_ELEMENTDATA 1;

template<typename T>
class ArrayList : public List<T> {
  public:
    ArrayList();
    ArrayList(int initialCapacity);
    int size();
    bool isEmpty();
    void add(T item);
    T set(int index, T item);
    T remove(T item);
    void clear();
    void insert(int index, T item);
  private:
    void rangeCheckForAdd(int index);
    int arraySize;
    int dataSize;
    T data[];
};

#endif
