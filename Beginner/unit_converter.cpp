#include <iostream>

constexpr unsigned int str2int(const char* str, int h = 0) {
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

int main(int argc, char const *argv[]) {
    
    char unit[2048];
    float value;

    std::cout << "What value ?\n";
    std::cin >> value;

    std::cout << "What unit ?\n";
    std::cin >> unit;

    
    switch (str2int(unit)) {
    
    case str2int("m"):
        
        std::cout << value << " " << unit << " is " << value / 1000 << " km\n"; 
        break;
    
    case str2int("km"):

        std::cout << value << " " << unit << " is " << value * 1000 << " m\n";
        break;
    
    case str2int("g"):

        std::cout << value << " " << unit << " is " << value / 1000 << " kg\n";
        break;
    
    case str2int("kg"):

        std::cout << value << " " << unit << " is " << value * 1000 << " g\n";
        break;
    
    default:

        std::cout << "The inputs were not recognized, make sure to have a float for value\n";
        break;
    }

    return 0;
}
