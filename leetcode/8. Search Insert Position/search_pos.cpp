#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int searchInsert(vector<int>& nums, int target) 
    {
        int left =0;
        int right = nums.size();
        while(left < right)
        {
            int mid = (left+ right)/2;
            if(nums[mid] == target)
                return mid;
            if(nums[mid] <target )
                left = mid + 1;
            else
                right =mid;
        }

        return left;
    }
};

int main()
{
    Solution sln;
    std::vector<int> nums {1,3,5,6};
    int target =4;
    std::cout<<"Ans ="<<sln.searchInsert(nums,target)<<std::endl;
    return (EXIT_SUCCESS);
}
