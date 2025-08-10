#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>


int main(int argc, char const *argv[])
{
    if (argc < 2) {
        std::cerr << "Error: Missing command. Use '--help' for options.\n";
        return 1;
    }

    if (!strcmp(argv[1], "--help")) {
        std::cout << "The different commands are: add, done, print\n";
        return 0;
    }

    if (!strcmp(argv[1], "add")) {
        if (argc < 3) {
            std::cerr << "Error: Missing item to add.\n";
            return 1;
        }

        std::ofstream myfile("list.txt", std::ios::app);
        if (myfile.is_open()) {
            myfile << argv[2] << "\n";
            myfile.close();
        } else {
            std::cerr << "Error: Could not open file for writing.\n";
        }
    }

    if (!strcmp(argv[1], "done")) {
        if (argc < 3) {
            std::cerr << "Error: Missing task to mark as done.\n";
            return 1;
        }

        std::ifstream infile("list.txt");
        if (!infile.is_open()) {
            std::cerr << "Error: Could not open file for reading.\n";
            return 1;
        }

        std::vector<std::string> lines;
        std::string line;
        bool found = false;

        while (std::getline(infile, line)) {
            if (line == argv[2] && !found) {
                found = true;
            } else {
                lines.push_back(line);
            }
        }
        infile.close();

        if (!found) {
            std::cout << "Item not found: " << argv[2] << "\n";
            return 0;
        }

        std::ofstream outfile("list.txt");
        if (!outfile.is_open()) {
            std::cerr << "Error: Could not open file for writing.\n";
            return 1;
        }
        for (const auto& updatedLine : lines) {
            outfile << updatedLine << "\n";
        }
        outfile.close();

        std::cout << "Item removed: " << argv[2] << "\n";
    }

    if (!strcmp(argv[1], "print")) {
        std::ifstream myfile("list.txt");
        if (myfile.is_open()) {
            std::string line;
            while (std::getline(myfile, line)) {
                std::cout << line << "\n";
            }
            myfile.close();
        } else {
            std::cerr << "Error: Could not open file for reading.\n";
        }
    }

    return 0;
}
