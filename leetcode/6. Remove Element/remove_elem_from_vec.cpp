#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        // this will also show how to remove safely item from vector
        for(std::vector<int>::iterator itr = nums.begin();
           itr != nums.end();)
        {
            if(*itr == val)
                itr = nums.erase(itr);
            else 
                ++itr;
        }
        return (nums.size());
    }
};

int main()
{
    std::vector<int> nums { 3,2,2,3};
    int num =3;

    Solution sl;
    int result = sl.removeElement(nums,num);
    std::cout<<"[";
    for(auto num : nums)
    {
        std::cout<<"\t"<<num;
    }
    std::cout<<"]";
    
    std::cout<<std::endl;
    return (EXIT_SUCCESS);
}