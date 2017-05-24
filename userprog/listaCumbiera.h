#include <stdio.h>
#include <stdlib.h>
#define MAX_SZ 2048

template <class T>
class listaCumbiera
{
    public:
        listaCumbiera();
        ~listaCumbiera();
        int put(T *elem);
        T *get(int n);
        void remove(int n);

    private:
        T *list[MAX_SZ];
};

template <class T>
listaCumbiera<T>::listaCumbiera()
{
    for(int i = 0; i < MAX_SZ; i++)
    {
        list[i] = NULL;
    }
}

template <class T>
listaCumbiera<T>::~listaCumbiera() { }

template <class T>
int listaCumbiera<T>::put(T *elem)
{
    int i = 0;
    while(list[i] != NULL)
        i++;
    if(i < MAX_SZ)
        list[i] = elem;
    else
        i = -1;

    DEBUG('L', "LISTA: devolvi %d\n", i);

    return i;
}

template <class T>
T *listaCumbiera<T>::get(int n)
{
    DEBUG('L', "LISTA: me pidieron get(%d)\n", n);
    if(n < 0 || n >= MAX_SZ)
        return NULL;
    return list[n];
}

template <class T>
void listaCumbiera<T>::remove(int n)
{
    DEBUG('L', "LISTA: me pidieron remove(%d)\n", n);
    if(n >= 0 && n < MAX_SZ)
        list[n] = NULL;
}

