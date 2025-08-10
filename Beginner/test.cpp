#include <iostream>
#include <cmath>
#include <string>
#include <chrono>
#include <thread>
#include <conio.h>  // For _kbhit() and _getch()

float escape_calculation(std::string name) {
    float result = 0;
    for (char c : name) {
        result += static_cast<float>(log2(static_cast<long double>(c)));
    }
    while (result > 1) result /= 10.0f;
    return static_cast<int>(result * 100) / 100.0f;
}


int main() {
    std::string bruh;
    /*
    std::string alphabet = "abcdefghijkmnopqrstuvwxyz";
    for (char e : alphabet) {
        bruh = e;
        std::cout << bruh << " = " << escape_calculation(bruh) << "\n";
    }
    return 0;
    */
   while (1) {
    std::getline(std::cin, bruh);
    std::cout << bruh << " = " << escape_calculation(bruh) << "\n";
   }
   
}