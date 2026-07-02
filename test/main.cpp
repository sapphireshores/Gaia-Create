#include <iostream>
#include "include/utils.h"

int main() {
    printLog("Starting test execution main binary sequence.");
    
    printGreeting();
    
    int val = 8;
    std::cout << "The square of " << val << " is " << computeSquare(val) << std::endl;
    
    printLog("Execution termination clean.");
    return 0;
}