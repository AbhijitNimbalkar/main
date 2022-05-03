#include <iostream>
#include <stdlib.h>
#include "list.h"

int main()
{
    list* obj = new list();

    obj->insert_start(50);
    obj->insert_start(40);
    obj->insert_start(30);
    obj->insert_start(20);
    obj->insert_start(10);
    obj->show("Display");

    obj->insert_end(60);
    obj->show("After insert end 60");

    obj->insert_after(50,55);
    obj->show("Inserting after 50 as 55");
    
    obj->insert_after(40,35);
    obj->show("Inserting befor 40 as 35");

    delete obj;
    obj = nullptr;
    return (EXIT_SUCCESS);    
}