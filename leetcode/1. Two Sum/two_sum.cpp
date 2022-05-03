#include <iostream>
#include <vector>
#include <map>
using namespace std;

vector<int> two_sum(vector<int>& nums, int target) 
{
    std::vector<int> output;
    std::map<int,int> map_values;
    // target = current_element + x
    //x is unknown 
    //target -x = current_element
    int index =0;
    for(auto& current_element : nums)
    {
        // 9 -2 = 7
        // 9 -7 = 2
        //9 -11 = -2
        //9  -15 = -6
         int x = target - current_element ;
         auto& itr = map_values.find(current_element);
         if(itr == map_values.end())
            map_values.insert(std::make_pair(x,index));
        else   
        {
            output.push_back(itr->second);
            output.push_back(index);
        }
        index++;
    }
    return output;
}
int main()
{
    std::vector<int> vec_input{2,7,11,15};
    std::vector<int> vec_output = two_sum(vec_input,9);
    for(auto& val : vec_output)
    {
        std::cout<<"["<<val<<"]=>";
    }
    std::cout<<std::endl;

    std::vector<int> vec_input1{3,2,4};
    std::vector<int> vec_output1 = two_sum(vec_input1,6);
    for(auto& val : vec_output1)
    {
        std::cout<<"["<<val<<"]=>";
    }
    std::cout<<std::endl;
 
    std::vector<int> vec_input2{3,3};
    std::vector<int> vec_output2 = two_sum(vec_input2,6);
    for(auto& val : vec_output2)
    {
        std::cout<<"["<<val<<"]=>";
    }
    std::cout<<std::endl;

    return (EXIT_SUCCESS);
}