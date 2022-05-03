//singly linked list with iterator pattern
#include <iostream>
using namespace std;

class LinkedList;
template <typename T>
class Node
{
    private:
    T data;
    Node* pNext;
    friend class LinkedList;
};

int main()
{
    return (EXIT_SUCCESS);
}