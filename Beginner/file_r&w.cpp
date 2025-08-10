#include <iostream>
#include <fstream>
#include <string>

int writer(std::string path, std::string text_to_be_added) {
    std::ofstream file(path, std::ios::app); // Open file in append mode
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    file << text_to_be_added << "\n";

    file.close();
    return 0;
}

int reader(std::string path) {
    std::ifstream file(path); // Open the file for reading
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return 1; // Exit with error
    }

    std::string line;
    while (std::getline(file, line)) { // Read line by line
        std::cout << line << std::endl;
    }

    file.close(); // Close the file
    return 0;
}

int main(int argc, char const *argv[])
{
    int return_value = 0;
    std::string path = "list.txt";
    std::string text_to_be_added = "Envoyer son CV";

    return_value = return_value || reader(path);
    std::cout << "\n";
    return_value = return_value || writer(path, text_to_be_added);
    std::cout << "\n";
    return_value = return_value || reader(path);

    return return_value;
}
