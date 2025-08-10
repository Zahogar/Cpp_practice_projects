#include <iostream>
#include <string>
#include <vector>
using namespace std;

void print_hangman(vector<string> hangman) {
    for (int i = 0; i < hangman.size(); i++){
        cout << hangman[i] << "\n";
    }
}

int main() {

    string word = "jeu";
    vector<string> hangman = {"  +---+", "  |   |", "      |", "      |", "      |", "      |", "========="};
    int errors = 0;
    string discovered_letters;
    for (int i = 0; i < word.size(); i++) {
        discovered_letters.append("_");
    }

    cout << "Let's play the hangman game !\n";

    while ((word != discovered_letters) && (errors < 6)) {
         
        char answer;
        bool has_letter = false;

        cout << discovered_letters << "\n";

        cout << "Guess a letter:";
        cin >> answer;

        for (int i = 0; i < word.size(); i++) {
            if (word[i] == answer) {
                discovered_letters[i] = answer;
                has_letter = true;
            }
        }

        if (!has_letter) errors++;

        switch (errors)
        {
        case 0:
            print_hangman(hangman);
            break;
        case 1:
            hangman[2][2] = 'o';
            print_hangman(hangman);
            break;
        case 2:
            hangman[3][2] = '|';
            print_hangman(hangman);
            break;
        case 3:
            hangman[3][1] = '/';
            print_hangman(hangman);
            break;
        case 4:
            hangman[3][3] = '\\';
            print_hangman(hangman);
            break;
        case 5:
            hangman[4][1] = '/';
            print_hangman(hangman);
            break;
        case 6:
            hangman[4][3] = '\\';
            print_hangman(hangman);
            break;
        default:
            cout << "This shouldn't be printed\n";
            break;
        }
    }

    if (errors >= 6) cout << "You lost the game !!\n";
}