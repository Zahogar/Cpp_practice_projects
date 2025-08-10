#include <iostream> 
#include <cassert>

int is_prime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i < n; i++) {
        if (!(n % i)) return 0;
    }
    return 1;
}

int main(int argc, char const *argv[])
{
    int n;
    
    std::cout << "Give me a number\n";
    std::cin >> n;

    if (is_prime(n)) std::cout << n << " is a prime number\n";
    else std::cout << n << " is NOT a prime number\n";

    if (n > 2) {
        std::cout << "Additionally, ";
        for (int i = 2; i < n; i++) {
            if (is_prime(i)) std::cout << i << " ";
        }
        std::cout << "are prime number\n";
    }

    return 0;
}
