#include <iostream>
#include <stdlib.h>

template <typename T>
class tlist; //forward decleration


template <typename T>
class node
{
   
    private: // onle accessible in chain
        T m_data;
        node<T>* m_next;
        node<T>* m_prev;
     public:
        node(T _data):m_data(_data),m_next(nullptr),m_prev(nullptr){};  

    friend class tlist<T>;
};

//template parameter list
template <typename T>
class tlist //as it is template list
{
    private:
        node<T>* m_head;
    //all the functions should be accessible outside
    public:
        tlist(){};
        ~tlist(){};
};

int main()
{
    tlist<int> list_of_roll_numbers;

    return (EXIT_SUCCESS);
}

