#ifndef List_h
#define List_h

template<typename T>
class List {
  public:
    virtual int size();
    virtual bool isEmpty();
    virtual void add(T item);
    virtual T set(int index, T item);
    virtual T remove(T item);
    virtual void clear();
    virtual void insert(int index, T item);
  private:
};

#endif
