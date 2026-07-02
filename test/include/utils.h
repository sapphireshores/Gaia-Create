#ifndef UTILS_H
#define UTILS_H

#include <iostream>

int computeSquare(int x);
void printGreeting();

inline void printLog(const std::string& msg) {
    std::cout << "[LOG] " << msg << std::endl;
}

#endif