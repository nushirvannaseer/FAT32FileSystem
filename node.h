#ifndef NODE_H
#define NODE_H

template <class T>
class node
{
    T data;
    node<T>* next;
    friend class FileSystem;
    template <class U> friend class linkedList;
    friend class systemWindow;

public:
    node()
        :next(nullptr)
    {}

    node(const node <T> & obj)
    {
        data = obj.data;
        next = obj.next;
    }

    node(const T & d, node<T>* n = nullptr)
        :data(d), next(n)
    {}

    ~node()
    {}

};

#endif // NODE_H
