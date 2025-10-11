#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <cstdlib>
#include <unistd.h>


// Useful functions

void toLowerInPlace(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
}

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = s.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + 1;
        end = s.find(delimiter, start);
    }
    
    tokens.push_back(s.substr(start));
    return tokens;
}


// Build-in cmds

void exit_cmd(std::vector<std::string>& args) {
    
    if (args.size() > 2) {
        std::cerr << "[ERROR] <from exit_cmd> - Too many arguments\n";
        return;
    } else if (args.size() == 2) {
        std::exit(std::atoi(args[1].c_str()));
        return;
    }

    std::exit(0);
    return;
}

void cd_cmd(std::vector<std::string>& args) {
    char cwd[1024];
    char* home = std::getenv("HOME");    

    if (args.size() > 2) {
        std::cerr << "[ERROR] <from cd_cmd> - Too many arguments\n";
    } else if (args.size() == 2) {

        if (args[1][0] == '~') {
            
            if (home == nullptr) {
                std::cerr << "[ERROR] <from cd_cmd> - Couldn't fetch 'HOME'\n";
                return;
            } else {
                args[1] = std::string(home) + args[1].substr(1);
            }
        }

        if (chdir(args[1].c_str()) == -1) {
            std::cerr << "[ERROR] <from cd_cmd> - Couldn't change directory, please verify the path\n";
        }

    } else {

        if (chdir(home) == -1) {
            std::cerr << "[ERROR] <from cd_cmd> - Couldn't change directory\n";
        }
    }

    return;
}

void help_cmd() {
    std::cout << "Here's the list of available commands:\n";
    std::cout << "exit [n]  -- exit the shell with the value n\n";
    std::cout << "cd [path] -- enter the directory at the end of the path\n";
    std::cout << "pwd       -- display the current working directory\n";
    std::cout << "help      -- display this menu\n";
    return;
}

void pwd_cmd(std::vector<std::string>& args) {
    char cwd[1024];
    
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << cwd << "\n";
    } else {
        std::cerr << "[ERROR] <from pwd_cmd> - Couldn't find the current working directory!\n";
    }

    return;
}


// CMD PARSERS

void bi_cmd_parser(std::vector<std::string>& split_vector) {

    if (split_vector.empty()) {
        std::cerr << "[ERROR] <from bi_parser> - Split vector shouldn't be empty!\n";
        return;
    }

    char cmd = split_vector[0][0];

    switch (cmd) {
    case 'e':
        exit_cmd(split_vector);
        break;
    case 'c':
        cd_cmd(split_vector);
        break;
    case 'h':
        help_cmd();
        break;
    case 'p':
        pwd_cmd(split_vector);
        break;
    
    default:
        std::cerr << "[ERROR] <from bi_parser> - Couldn't find the command. Use the command 'help' to see the list of available commands.\n";
        break;
    }
    return;
}


void fst_cmd_parser(std::vector<std::string>& split_vector) {

    std::unordered_set<std::string> build_in_cmd = {"exit", "cd", "help", "pwd"};

    if (build_in_cmd.find(split_vector[0]) != build_in_cmd.end()) {
        bi_cmd_parser(split_vector);
    } else {
        std::cerr << "[ERROR] <from fst_parser> - Couldn't find the command. Use the command 'help' to see the list of available commands.\n";
    }
    return;
}


// MAIN

int main(int argc, char const *argv[]) {
    std::string answer;
    std::vector<std::string> split_vect;

    std::cout << " > ";

    while (1) {
        std::getline(std::cin, answer);

        if (answer.empty()) {
            std::cout << " > ";
            continue;
        }

        split_vect = split(answer, ' ');

        if (split_vect.empty()) {
            std::cerr << "[ERROR] <from main> - Split vector shouldn't be empty!\n";
            return 1;
        } else {
            fst_cmd_parser(split_vect);
        }

        answer = "";
        std::cout << " > ";        
    }

    return 0;
}
