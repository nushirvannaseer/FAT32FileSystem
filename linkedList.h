#ifndef LIST_H
#define LIST_H
#include "node.h"

template <class T>
class linkedList
{
    node <T>* head, *tail;//dummy head and dummy tails
    int size;
    friend class FileSystem;
    friend class systemWindow;

public:
    linkedList()
    {
        head = new node<T>();
        tail = new node<T>();
        head->next = tail;
        tail->next = nullptr;
        size = 0;
    }

    void insertAtEnd(const T& element)
    {
        node <T>* temp = head;
        while (temp != tail)
        {
            if (temp->next == tail)
            {
                node <T>* newElement = new node<T>(element, tail);
                temp->next = newElement;
                size++;
                return;
            }
            temp = temp->next;
        }
    }

    void insertAtEnd(node<T> &address)//this function changes the pointers only
    {
        node <T>* temp = head;
        while (temp != tail)
        {
            if (temp->next == tail)
            {
                address.next = tail;
                temp->next = &address;
                size++;
                return;
            }
            temp = temp->next;
        }

        size++;
    }

    void insertAtStart(const T& element)
    {
        node<T>* newElement = new node<T>(element, head->next);
        head->next = newElement;
        size++;
    }

    void insertAtStart(node<T> &address)//this function changes the pointers only
    {
        address->next = head->next;
        head = address;
        size++;
    }

    void removeFromEnd()
    {
        if (size==0)
        {
            return;
        }

        node <T>* temp = head;
        node <T>* prev = head;
        while (temp != tail)
        {
            if (temp->next == tail)
            {
                prev->next = tail;
                delete temp;
            }
            prev = temp;
            temp = temp->next;
        }

        size--;
    }

    void removeFromStart()
    {
        if (size == 0)
            return;

        node <T>* toDel = head->next;
        head->next = toDel->next;
        delete toDel;
        size--;
    }

    bool search(const T& element)const
    {
        node<T>* temp = head->next;
        for (int i = 0; i < size; i++)
        {
            if (temp->data == element)
                return true;

            temp = temp->next;
        }

        return false;
    }

    bool removeElement(const T& element)
    {
        if (search(element) == true)
        {
            node<T>* temp = head->next;
            node<T>* prev = head;

            while (temp != tail)
            {
                if (temp->data == element)
                {
                    prev->next = temp->next;
                    delete temp;
                    size--;
                    return true;
                }

                prev = temp;
                temp = temp->next;
            }
        }

        return false;
    }

    void removeElement(node <T>* &address)//this one just reallocates pointers
    {
        node<T>* temp = head->next;
        node<T>* prev = head;

        while (temp != tail)
        {
            if (temp == address)
            {
                prev->next = temp->next;
                size--;
                return;
            }

            prev = temp;
            temp = temp->next;
        }
    }

    int getSize()const
    {
        return size;
    }

    bool isEmpty()const
    {
        if (head->next == tail)
            return true;

        return false;
    }

    ~linkedList()
    {
        /*node<T>* prev = head;
        node<T>* curr_pointer = head->next;

        while (curr_pointer != nullptr)
        {
            prev = curr_pointer;
            curr_pointer = curr_pointer->next;
            delete prev;
        }*/
    }
};



#endif // LIST_H
