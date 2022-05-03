// /*

//     [ capture list] return type => (parameter list)
//     {
//         function body;
//     }
//     capture list = local parameters which needs to passed
// */


#include <iostream>

void TestFirstLambdaScenario()
{
    int i =20, j =10;

    auto compare_ints = [] (int a1, int a2)
    {
        return a1 > a2;
    };

    bool bResult = compare_ints(i,j);
    std::cout << (bResult == true ? "i is greater than j" :"j is greater than i")<<std ::endl;
}
void TestSecondLambdaScenario()
{
    //[capture list] (parameters)->return type{}
    int i =10;
    int j =20;
    auto compare_ints = [i] (int j)->bool
    {
        return i > j ;
    };
    bool bResult = compare_ints(j);
    std::cout << (bResult == true ? "i is greater than j" :"j is greater than i")<<std ::endl;
}
int main()
{
   // TestFirstLambdaScenario();
    TestSecondLambdaScenario();
    return EXIT_SUCCESS;
}
