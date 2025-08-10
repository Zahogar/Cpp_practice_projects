#include <iostream>

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    int answer = rand() % 100 +1;
    int n;
    std::cout << "The number your looking for is between 1 and 100\n";
    
    do
    {
        std::cout << "Try guessing the number: ";
        std::cin >> n;
        if (n > answer) std::cout << "too high\n";
        else if (n < answer) std::cout << "too low\n";
    } while (n != answer);
    
    std::cout << "Bravo, you found it. It was obviously " << answer << "\n";

    return 0;
}
