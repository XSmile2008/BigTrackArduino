#ifndef List_h
#define List_h

template<typename T>
class List {
  public:
    virtual void add(T item);
    virtual void add(int index, T item);
    virtual T set(int index, T item);
    virtual T get(int index);
    virtual T remove(int index);
    virtual void clear();

    virtual int size();
    virtual bool isEmpty();
  private:
};

#endif
