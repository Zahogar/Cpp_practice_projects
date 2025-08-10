#include <iostream>
#include <string>

// ANSI color codes
#define RESET               "\033[0m"
#define BLACK               "\033[30m"
#define RED                 "\033[31m"
#define GREEN               "\033[32m"
#define YELLOW              "\033[33m"
#define BLUE                "\033[34m"
#define MAGENTA             "\033[35m"
#define CYAN                "\033[36m"
#define WHITE               "\033[37m"

#define HIGHLIGHTED_RED     "\033[41m"
#define HIGHLIGHTED_GREEN   "\033[42m"
#define HIGHLIGHTED_YELLOW  "\033[43m"
#define HIGHLIGHTED_BLUE    "\033[44m"
#define HIGHLIGHTED_MAGENTA "\033[45m"
#define HIGHLIGHTED_CYAN    "\033[46m"
#define HIGHLIGHTED_WHITE   "\033[47m"

#define LIGHTER_BLACK       "\033[90m"
#define LIGHTER_RED         "\033[91m"
#define LIGHTER_GREEN       "\033[92m"
#define LIGHTER_YELLOW      "\033[93m"
#define LIGHTER_BLUE        "\033[94m"
#define LIGHTER_MAGENTA     "\033[95m"
#define LIGHTER_CYAN        "\033[96m"

// ANSI text style codes
#define BOLD                "\033[1m"
#define DARKER              "\033[2m"
#define ITALIC              "\033[3m"
#define UNDERLINE           "\033[4m"
#define BLINKING            "\033[5m"
#define HIGHLIGHTED         "\033[7m"
#define BLACKED             "\033[8m"
#define CROSSED             "\033[9m"
#define DOUBLE_UNDERLINE    "\033[21m"
#define UPPERLINE           "\033[53m"


std::string color_parser(std::string color) {
    if (color[0] == 'h') {
        color = color.substr(12, color.length()-12);
        if (color == "red") return HIGHLIGHTED_RED;
        if (color == "green") return HIGHLIGHTED_GREEN;
        if (color == "yellow") return HIGHLIGHTED_YELLOW;
        if (color == "blue") return HIGHLIGHTED_BLUE;
        if (color == "magenta") return HIGHLIGHTED_MAGENTA;
        if (color == "cyan") return HIGHLIGHTED_CYAN;
        if (color == "white") return HIGHLIGHTED_WHITE;
        else return "\n/// COLOR ERROR ///\n";
    }
    if (color[0] == 'l') {
        color = color.substr(8, color.length()-8);
        if (color == "black") return LIGHTER_BLACK;
        if (color == "red") return LIGHTER_RED;
        if (color == "green") return LIGHTER_GREEN;
        if (color == "yellow") return LIGHTER_YELLOW;
        if (color == "blue") return LIGHTER_BLUE;
        if (color == "magenta") return LIGHTER_MAGENTA;
        if (color == "cyan") return LIGHTER_CYAN;
        else return "\n/// COLOR ERROR ///\n";
    }
    else {
        if (color == "black") return BLACK;
        if (color == "red") return RED;
        if (color == "green") return GREEN;
        if (color == "yellow") return YELLOW;
        if (color == "blue") return BLUE;
        if (color == "magenta") return MAGENTA;
        if (color == "cyan") return CYAN;
        if (color == "white") return WHITE;
        else return "\n/// COLOR ERROR ///\n";
    }
}

int main() {
    
    
    // Colors with styles
    std::cout << RED << BOLD << "This text is red and bold!" << RESET << std::endl;
    std::cout << GREEN << ITALIC << "This text is green and italic!" << RESET << std::endl;
    std::cout << BLUE << UNDERLINE << "This text is blue and underlined!" << RESET << std::endl;
    std::cout << YELLOW << CROSSED << "This text is yellow and crossed out!" << RESET << std::endl;
    
    // Multiple styles together
    std::cout << MAGENTA << BOLD << UNDERLINE << BLINKING << "This text is magenta, bold and underlined!" << RESET << std::endl;
    
    /*
    for (int i = 0; i < 100; i++) {
        std::cout << "\033[" << i << "m" << "Manger des pommes" << "\033[0m" << " for i = " << i << "\n";
    }
    */
    std::string str = "Manger des pommes";

    std::cout << HIGHLIGHTED << RED << str << RESET << "\n";
    std::cout << HIGHLIGHTED_RED << str << RESET << "\n";


    //std::cout << str.substr(7, str.length()-7) << "\n";

    std::cout << color_parser("black") << str << RESET << "\n";
    return 0;
}