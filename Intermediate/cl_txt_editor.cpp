#include <iostream>
#include <conio.h>      // For _getch()
#include <windows.h>    // For console control
#include <vector>
#include <string>

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Move cursor to (x, y)
void moveCursor(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, pos);
}

// Clear screen
void clearScreen() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD written;
    FillConsoleOutputCharacter(hConsole, ' ', cellCount, {0, 0}, &written);
    SetConsoleCursorPosition(hConsole, {0, 0});
}

// Editor state
struct Editor {
    vector<string> buffer = {""};
    int cursorX = 0;
    int cursorY = 0;
} editor;

void drawEditor() {
    clearScreen();
    for (const string& line : editor.buffer) {
        cout << line << "\n";
    }
    moveCursor(editor.cursorX, editor.cursorY);
}

void insertChar(char c) {
    string& line = editor.buffer[editor.cursorY];
    line.insert(editor.cursorX, 1, c);
    editor.cursorX++;
}

void backspace() {
    if (editor.cursorX > 0) {
        string& line = editor.buffer[editor.cursorY];
        line.erase(editor.cursorX - 1, 1);
        editor.cursorX--;
    }
}

int main() {
    bool running = true;
    drawEditor();

    while (running) {
        int ch = _getch(); // No buffering or echo

        switch (ch) {
            case 3: // Ctrl+C
                running = false;
                break;
            case 8: // Backspace
                backspace();
                break;
            case 13: // Enter
                editor.buffer.insert(editor.buffer.begin() + editor.cursorY + 1, "");
                editor.cursorY++;
                editor.cursorX = 0;
                break;
            default:
                if (ch >= 32 && ch <= 126) { // Printable ASCII
                    insertChar((char)ch);
                }
                break;
        }

        drawEditor();
    }

    clearScreen();
    cout << "Goodbye!" << endl;
    return 0;
}
