#include <iostream>

int main() {

    float weight;
    float height;

    std::cout << "What is your weight (in kg) ?\n";
    std::cin >> weight;

    std::cout << "What is your height (in m) ?\n";
    std::cin >> height;

    std::cout << "You BMI is " << weight / (height * height) << " !!!\n";
}