#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>


int main(int argc, char const *argv[]) {
    if (argc < 4) {
        std::cout << "You gave too few arguments.\nThe call should look like: ./file.exe float unit1 unit2\n";
        return 1;
    }

    std::vector<std::string> units = {"Fahrenheit", "Kelvin", "Celsius"};

    if (!std::count(units.begin(), units.end(), argv[2]) || !std::count(units.begin(), units.end(), argv[3])) {
        std::cout << "You didn't gave a proper unit.\nExpected Fahrenheit, Kelvin, or Celsius\n";
        return 1;
    }

    if (!strcmp(argv[2], "Celsius")) {
        if (!strcmp(argv[3], "Kelvin")) std::cout << std::stof(argv[1]) + 273.15 << "\n";
        if (!strcmp(argv[3], "Fahrenheit")) std::cout << std::stof(argv[1]) * 1.8 + 32 << "\n";
    }

    if (!strcmp(argv[2], "Fahrenheit")) {
        if (!strcmp(argv[3], "Kelvin")) std::cout << (std::stof(argv[1]) - 32) / 1.8 + 273.15 << "\n";
        if (!strcmp(argv[3], "Celsius")) std::cout << (std::stof(argv[1]) - 32) / 1.8 << "\n";
    }

    if (!strcmp(argv[2], "Kelvin")) {
        if (!strcmp(argv[3], "Celsius")) std::cout << std::stof(argv[1]) - 273.15 << "\n";
        if (!strcmp(argv[3], "Fahrenheit")) std::cout << (std::stof(argv[1]) - 273.15) * 1.8 + 32 << "\n";
    }

    return 0;
}
