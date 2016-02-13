#ifndef ArrayList_h
#define ArrayList_h
#include "List.h"

template<typename T>
class ArrayList : public List<T> {
  public:
    ArrayList();
    ArrayList(uint8_t initialCapacity);
    ~ArrayList();

    void add(T item);
    void add(uint16_t index, T item);
    T set(uint16_t index, T item);
    T get(uint16_t index);
    T remove(uint16_t index);
    void clear();

    uint16_t size();
    bool isEmpty();
    void trimToSize();

    //@Deprecated
    void print();
  private:

    bool rangeCheck(uint16_t index);
    void ensureCapacity(uint16_t index);
    void grow(uint16_t minCapacity);

    uint16_t capacity;
    uint16_t valuesSize;
    T* values;

    static const uint8_t DEFAULT_CAPACITY = 1;
};

#endif
