#ifndef ArrayList_h
#define ArrayList_h
#include "List.h"
#include "Arduino.h"

template<typename T>
class ArrayList : public List<T> {
  public:
    ArrayList();
    ArrayList(int initialCapacity);
    ~ArrayList();

    void add(T item);
    void add(int index, T item);
    T set(int index, T item);
    T get(int index);
    T remove(int index);
    void clear();

    int size();
    bool isEmpty();
    void trimToSize();

    //@Deprecated
    void print();
  private:

    bool rangeCheck(int index);
    void ensureCapacity(int index);
    void grow(int minCapacity);

    int capacity;
    int valuesSize;
    T* values;

    static const int DEFAULT_CAPACITY = 1;
};

#endif
