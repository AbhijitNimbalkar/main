#include <iostream>
#include <queue>

int main()
{
    std::queue<int> q_objects;    

    for (size_t i = 1; i <= 10; i++)
    {
        /* code */
        q_objects.push(i);
    }
    
    std::cout<<"BACK  = "<<q_objects.back()<<std::endl;
    std::cout<<"FRONT = "<<q_objects.front()<<std::endl;
    q_objects.pop();
    std::cout<<"AFTER POPPED OUT"<<std::endl;
    std::cout<<"BACK  = "<<q_objects.back()<<std::endl;
    std::cout<<"FRONT = "<<q_objects.front()<<std::endl;

    return (EXIT_SUCCESS);
}