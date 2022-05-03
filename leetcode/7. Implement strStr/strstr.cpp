#include <iostream>
#include <string>

using namespace std;
class Solution {
public:

    bool match(int starting_index,string haystack,string needle)
    {
        for(char c:needle)
        {
            if(haystack.size() <= starting_index )
            {
                return false;
            }
            else if(haystack[starting_index++] != c )
                return false;
        }
        //string all chars are matching
        return true;
    }
    int strStr(string haystack, string needle)
    {
        if(needle.size() <= 0)
            return 0;

        for(int j =0;j <haystack.size();j++)
        {
            if(haystack[j] == needle [0])
            {
                if(match(j,haystack,needle))
                {
                    return j;
                }
            }
        }
        return -1;
    }
};

int main()
{
    Solution s;
    std::cout <<"Ans = "<< s.strStr("mississipi","issip")<<std::endl;
     std::cout <<"Ans = "<< s.strStr("ibba","ab")<<std::endl;
    return (EXIT_SUCCESS);
}