#include <iostream>
#include <cassert>
#include <typeinfo>

int main(int argc, char const *argv[])
{
    if (argc > 5) {
        std::cout << "There should be only 3 arguments. Reminder: use x or `*` for multiplication\n";
        return 1;
    }

    int result;

    switch (argv[2][0])
    {
    case '+':
        result = atoi(argv[1]) + atoi(argv[3]);
        break;
    
    case '-':
        result = atoi(argv[1]) - atoi(argv[3]);
        break;

    case '*':
        result = atoi(argv[1]) * (argv[2][2] - '0');
        break;

    case 'x':
        result = atoi(argv[1]) * atoi(argv[3]);
        break;
    
    case '/':
        result = atoi(argv[1]) / atoi(argv[3]);
        break;

    default:
        std::cout << argv[2] << " is an unknown operation\n";
        break;
    };

    std::cout << result;

    return 0;
}