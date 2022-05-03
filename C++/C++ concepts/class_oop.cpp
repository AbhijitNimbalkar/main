#include <iostream>
#include <string>
#include <stdlib.h>
class Person
{
private:
    std::string _name;
public:
    Person()
    {
        std::cout<<__PRETTY_FUNCTION__<<std::endl;   
    }
    Person(std::string name)
    {
        std::cout<<__PRETTY_FUNCTION__<<std::endl;
      _name = name;
    }
    virtual void display()
    {
        std::cout<<"name = "<<_name<<std::endl;
    }
};
class Student : public Person
{
private:
    int _id;
public:
    Student(int id,std::string name):Person(name),_id(id)
    {
        std::cout<<__PRETTY_FUNCTION__<<std::endl;   
    }
    void display()
    {
        Person::display();
        std::cout<<"id = "<<_id<<std::endl;
    }
};
int main()
{
    Student o{10,"Abhijit"};
    o.display();
    return (EXIT_SUCCESS);
}