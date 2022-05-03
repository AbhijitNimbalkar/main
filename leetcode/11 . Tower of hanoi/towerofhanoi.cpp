#include <iostream>
#include <stack>
using namespace std;

void display(std::stack<int>& s)
{
   if(s.empty())
    return;
   int top = s.top();
   s.pop();
   std:cout<<top<<std::endl;
   display(s);
   s.push(top);
}

void TowerOfHanoi(std::stack<int>S,std::stack<int>A ,std::stack<int>D)
{
    
}

int main()
{
    std::stack<int> Tsource;
    std::stack<int> TDestination;
    std::stack<int> TAuxilary;
    Tsource.push(5);
    Tsource.push(4);
    Tsource.push(3);
    Tsource.push(2);
    Tsource.push(1);
    display(Tsource);
    return (EXIT_SUCCESS);
}