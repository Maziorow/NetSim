#include <iostream>
#include "package.hpp"
#include "storage_types.hpp"
#include<storage_types.hpp>

int main(){
    Package p1;
    Package p2;
    std::cout << "IsAssignedIDLowest?" << std::endl;
    std::cout << (p1.get_id() == 1 and p2.get_id() == 2) << std::endl;
}