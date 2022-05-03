#include <iostream>
#include <map>
#include <string>
using namespace std;
struct ltstr
{
  bool operator()(const int l, const int r) const
  {
    return l > r;
  }
};

int main()
{
    multimap<int,string,ltstr> mapOfNames;
    mapOfNames.insert(std::make_pair(100,"AAA"));
    mapOfNames.insert(std::make_pair(98,"BBB"));
    mapOfNames.insert(std::make_pair(99,"CCC"));
    for(const auto c :mapOfNames)
        cout<<"name = \t"<<c.first<<"\t marks = \t"<<c.second<<std::endl;

    return (EXIT_SUCCESS);
}