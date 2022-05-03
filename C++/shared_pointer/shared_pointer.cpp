#include <iostream>
#include <memory>
#include <string>

class CEmployee
{
public:
    int m_id;
    std::string m_name;
    CEmployee(int _id,std::string _name):m_id(_id),m_name(_name)
    {
        std::cout<<"Inside Ctor"<<std::endl;
    }
    ~CEmployee()
    {
        std::cout<<"Inside Dtor"<<std::endl;
    }
};

void modify_object(std::shared_ptr<CEmployee> o)
{
    o->m_id =20;
    o->m_name = "Vaishnavi";
    std::cout<<"Use count1 ="<<o.use_count()<<std::endl;
}
void print_object(std::shared_ptr<CEmployee> o)
{
    std::cout<<"ID = "<<o->m_id<<std::endl;
    std::cout<<"NAME = "<<o->m_name<<std::endl;
    std::cout<<"Use count2 ="<<o.use_count()<<std::endl;
}

int main()
{
    auto ptrEmployee  = std::make_shared<CEmployee>( 1,"Abhijit");
    print_object(ptrEmployee);
    auto ptrEmployee1 = ptrEmployee ;
    modify_object(ptrEmployee);
    print_object(ptrEmployee);

    print_object(ptrEmployee1);
    std::cout<<"Use count ="<<ptrEmployee1.use_count()<<std::endl;
    return (EXIT_SUCCESS);
}