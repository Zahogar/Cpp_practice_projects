#include <vector>
#include <string>
#include <iostream>
#include <cassert>

void print(std::vector<std::vector<int>> vec) {
    for (int i = 0; i < vec.size(); i++) {
        std::string S = "";
        for (int j = 0; j < vec.size(); j++) {
            if (i < 2) {
                S += "\033[4m";
                if (vec[i][j] == 1) {
                    if (j < 2) S += "x|";
                    else S += "x";
                }
                else if (vec[i][j] == 2) {
                    if (j < 2) S += "o|";
                    else S += "o";
                }
                else {
                    if (j < 2) S += " |";
                    else S += " ";
                }
                S += "\033[0m";
            }
            else {
                if (vec[i][j] == 1) {
                    if (j < 2) S += "x|";
                    else S += "x";
                }
                else if (vec[i][j] == 2) {
                    if (j < 2) S += "o|";
                    else S += "o";
                }
                else {
                    if (j < 2) S += " |";
                    else S += " ";
                }
            }
        }
        std::cout << S << "\n";
    }
}

bool is_full(std::vector<std::vector<int>> vec) {
    for (int i = 0; i < vec.size(); i++) {
        for (int j = 0; j < vec.size(); j++) {
            if (!vec[i][j]) return false;
        }
    }
    return true;
}

int player_x_won(std::vector<std::vector<int>> vec) {

    std::vector<int> player_1 = {1, 1, 1};
    std::vector<int> player_2 = {2, 2, 2};

    // verify lines
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == player_1) return 1;
        else if (vec[i] == player_2) return 2;
    }
    
    // verify columns
    for (int j = 0; j < vec.size(); j++) {
        std::vector<int> a = {};
        for (int i = 0; i < vec.size(); i++) {
            a.insert(a.end(), vec[i][j]);
        }

        if (a == player_1) return 1;
        else if (a == player_2) return 2;
    }

    //verify diagonals
    std::vector<int> diag1 = {};
    std::vector<int> diag2 = {};

    for (int i = 0; i < vec.size(); i++) {
        diag1.insert(diag1.end(), vec[i][i]);
        diag2.insert(diag2.begin(), vec[i][i]);
    }
    if (diag1 == player_1 || diag2 == player_1) return 1;
    else if (diag1 == player_2 || diag2 == player_2) return 2;

    return 0;
}

int main(int argc, char const *argv[]) {
    
    std::vector<std::vector<int>> grid = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    int current_player = 1;

    while (1) {
        std::cout << "Where do you want to play player " << current_player <<" ?\n";
        print(grid);

        int input;
        std::cin >> input;

        assert(10 < input && input < grid.size()*11+1);

        if (!grid[input/10-1][input%10-1]) grid[input/10-1][input%10-1] = current_player;

        if (is_full(grid)) {
            std::cout << "Draw !\n";
            break;
        }

        if (player_x_won(grid)) {
            std::cout << "Player " << player_x_won(grid) << " won !\n";
            break;
        }

        current_player = (current_player == 1) ? 2 : 1;
    }
    return 0;
}
