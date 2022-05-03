// /*

//     [ capture list] return type => (parameter list)
//     {
//         function body;
//     }
//     capture list = local parameters which needs to passed
// */


// #include <iostream>

// void TestFirstLambdaScenario()
// {
//     int i =20, j =10;

//     auto compare_ints = [] (int a1, int a2)
//     {
//         return a1 > a2;
//     };

//     bool bResult = compare_ints(i,j);
//     std::cout << (bResult == true ? "i is greater than j" :"j is greater than i")<<std ::endl;
// }
// void TestSecondLambdaScenario()
// {
//     //[capture list] (parameters)->return type{}
//     int i =10;
//     int j =20;
//     auto compare_ints = [i] (int j)->bool
//     {
//         return i > j ;
//     };
//     bool bResult = compare_ints(j);
//     std::cout << (bResult == true ? "i is greater than j" :"j is greater than i")<<std ::endl;
// }
// int main()
// {
//    // TestFirstLambdaScenario();
//     TestSecondLambdaScenario();
//     return EXIT_SUCCESS;
// }


#include<iostream>
#include<vector>
#include <algorithm>

#include <stdio.h>
#include <string.h>

using namespace std;
class emp
{
    int id;
    string name;
    public:
    emp(int i, string n) :id(i), name(n) {}
    void print()
    {
        cout << endl << id << name;
    }
    friend bool MyCompare(emp l ,emp r);
};
class emppool
{
public:
    static void display(std::vector<emp>e)
    {
        for (auto itr : e)
        {
            itr.print();
        }
    }
};

bool MyCompare(emp l ,emp r)
{
    return (l.name < r.name) ?  true: false;
}
int  main()
{
    std::vector<emp> E;
    E.push_back(emp(10,"Shzaine"));
    E.push_back(emp(12, "Aisha"));
    E.push_back(emp(9, "Naina"));
    std::sort(E.begin(),E.end(),MyCompare);

    emppool::display(E);
    return (EXIT_SUCCESS);
}