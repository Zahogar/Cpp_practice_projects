#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>


// Useful functions

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
    std::cout << "\n===== Mini Shell Help =====\n\n";
    
    std::cout << "Built-in Commands:\n";
    std::cout << "  exit [n]     Exit the shell with optional exit code n\n";
    std::cout << "  cd [path]    Change directory to path (use ~ for home)\n";
    std::cout << "  pwd          Print current working directory\n";
    std::cout << "  help         Display this help message\n\n";
    
    std::cout << "External Commands:\n";
    std::cout << "  Any executable in your PATH can be run\n";
    std::cout << "  Examples: ls, cat, echo, grep, ps, top, mkdir, etc.\n\n";
    
    std::cout << "Usage:\n";
    std::cout << "  Simply type a command and press Enter\n";
    std::cout << "  Example: ls -l\n";
    std::cout << "  Example: cd /home\n\n";
    
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

void ext_cmd_parser(std::vector<std::string>& split_vector){
    pid_t pid = fork();
    int status;
    std::vector<char*> args;

    //conversion of data type (std::string -> char*)
    for (size_t i = 0; i < split_vector.size(); i++) {
        args.push_back(const_cast<char*>(split_vector[i].c_str()));
    }
    args.push_back(nullptr);

    // child perspective
    if (pid == 0) {
        execvp(args[0], args.data());
        std::cerr << "[ERROR] <from ext_parser> Unrecognized command. Use the command 'help' to see the list of available commands.\n";
        std::exit(1);
    } else { // parent perspective
        waitpid(pid, &status, 0);
    }

    return;
}

void fst_cmd_parser(std::vector<std::string>& split_vector) {

    std::unordered_set<std::string> build_in_cmd = {"exit", "cd", "help", "pwd"};

    if (build_in_cmd.find(split_vector[0]) != build_in_cmd.end()) {
        bi_cmd_parser(split_vector);
    } else {
        ext_cmd_parser(split_vector);
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
