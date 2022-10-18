#include <iostream>
#include <vector>
#include <string>
#include "SkipList.hpp"

int main()
{
    SkipList<int, int> testq;
    for(double i = 0; i  < 5; ++i)
        testq.insert(i, i);
//    testq.print();
    testq.clear();
    //testq.print();
    return 0;
}

