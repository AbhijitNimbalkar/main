#include <iostream>
#include <string>
using namespace std;

/*
Approach: A number in Roman Numerals is a string of these symbols written in descending order(e.g. M’s first, followed by D’s, etc.). 
However, in a few specific cases, to avoid four characters being repeated in succession(such as IIII or XXXX), subtractive notation is often used as follows: 

I placed before V or X indicates one less, so four is IV (one less than 5) and 9 is IX (one less than 10).
X placed before L or C indicates ten less, so forty is XL (10 less than 50) and 90 is XC (ten less than a hundred).
C placed before D or M indicates a hundred less, so four hundred is CD (a hundred less than five hundred) and nine hundred is CM (a hundred less than a thousand).
Algorithm to convert Roman Numerals to Integer Number:  

Split the Roman Numeral string into Roman Symbols (character).
Convert each symbol of Roman Numerals into the value it represents.
Take symbol one by one from starting from index 0: 
If current value of symbol is greater than or equal to the value of next symbol, then add this value to the running total.
else subtract this value by adding the value of next symbol to the running total.

*/
class Solution {
public:  
    int findValue(char m) 
    {
        if (m == 'I') return 1;
        else if (m == 'V') return 5;
        else if (m == 'X') return 10;
        else if (m == 'L') return 50;
        else if (m == 'C') return 100;
        else if (m == 'D') return 500;
        else if (m == 'M') return 1000;
        else return 0;
    }  
    
    int romanToInt(string s) 
    {
        int sum = 0;
        for (int i = 0; i < s.size(); i++) 
        {
            int check1 = 0, check2 = 0;
            check1 = findValue(s[i]); // 1000    100    1000    10      100     1      5
            check2 = findValue(s[i+1]);// 100    1000   10      100     1       5      0

            if (check1 >= check2) 
            {
                sum += check1;
            }
            else if (check1 < check2) 
            {
                sum += (check2 - check1);   // IV  = V -I so IV is one no i.e. 4 hence extra i++
                i++;
            }
        }
        return sum;
    }
};
int main()
{
    Solution o;
    std::cout<<"Answer XII= "<<o.romanToInt("MCMXCIV")<<std::endl;
    return (EXIT_SUCCESS);
}