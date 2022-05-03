#include <iostream>
#include <vector>
#include <map>
using namespace std;


 int singleNumber(vector<int>& nums) {
        map<int,int> map_of_numbers;
        for(auto i : nums){
            auto insert_status= map_of_numbers.insert(std::make_pair(i,i));
            if(insert_status.second == false){
                map_of_numbers.erase(insert_status.first);
             }
        }
        if(map_of_numbers.empty() == false)
          return map_of_numbers[0];
         return -1;
  }
int main()
{
    vector<int> vec_ints = {2,2,1};
    int output = singleNumber(vec_ints);
    std::cout<<"Number is = " <<output<<std::endl;
    return (EXIT_SUCCESS);
}
