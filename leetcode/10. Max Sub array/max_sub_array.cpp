#include <iostream>
#include <assert.h>
#include <vector>

using namespace std;

class Solution {
public:
    int maxSubArray(vector<int>& nums) {
            int maxSum = nums[0];
            int sum = 0;
            for (int index = 0; index < nums.size(); index++){
                sum += nums[index];
                if (sum < nums[index])
                {
                    sum = nums[index];
                }
                if (maxSum < sum){
                    maxSum = sum;
                }
            }
            return maxSum;
    }
};
int main()
{

    Solution sln;
    std::vector<int> vec_problem{-2,1,-3,4,-1,2,1,-5,4};

    std::cout<<"Answer = \t"<<sln.maxSubArray(vec_problem)<<std::endl;

    return  (EXIT_SUCCESS);
}