#include <iostream>
#include <string.h>
#include <stdio.h>

template <typename T>
T compare (T a ,T b)
{
    return a> b? a :  b;
}

template <unsigned c1,unsigned c2>
int compare_strings(const char (&str1)[c1], const char(&str2)[c2])
{
    return strcmp(str1,str2);
}

template <unsigned c>
void print_array(int  (arr) [])
{
    for (size_t i = 0; i < c; i++){
        /* code */
        std::cout<<"["<<arr[i]<<"]"<<std::endl;
    }
}

int main()
{
    //std::cout <<"Results of compare function  = "<<compare (10,20)<<std::endl;
   // std::cout<<"Compare strings results "<< compare_strings("AAAA","AAAA") <<std::endl;
   //int arr [] = {1,2,3,4};
  //  print_array<4>(arr);

    return (EXIT_SUCCESS);
}