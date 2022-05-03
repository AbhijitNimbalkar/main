/*
X at the start   =123
        x=      12      reverse=        3       digit   3
        x=      1       reverse=        32      digit   2
        x=      0       reverse=        321     digit   1
No. It does not :(
*/
#include <iostream>
bool isPalindrome(int x) 
{
    long int digit,reverse=0;
    int temp=x;
    std::cout<<"X at the start\t ="<<x<<std::endl;
    while(x>0)
    {
        digit=x%10;
        reverse=(reverse*10)+digit;
        x=x/10;
        std::cout << "\tx=\t"<<x<<"\treverse=\t"<<reverse<<"\tdigit\t"<<digit<<std::endl;
    }
    if(temp==reverse)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}
int main()
{
    isPalindrome(123) ? 
        std::cout<<"Yes. It is :)" 
    : std::cout<<"No. It does not :( ";
    return (EXIT_SUCCESS);
}