// ============================================================================
// KeyProcessor.cpp - Implementação completa
// ============================================================================
#include "KeyProcessor.h"
#include <cctype>
#include <sstream>
#include <mutex>
#include <thread>

KeyProcessor::KeyProcessor() {
    initializeKeyMaps();
}

void KeyProcessor::initializeKeyMaps() {
    // Teclas especiais
    specialKeys[VK_SPACE] = "SPACE";
    specialKeys[VK_RETURN] = "ENTER";
    specialKeys[VK_TAB] = "TAB";
    specialKeys[VK_BACK] = "BACKSPACE";
    specialKeys[VK_DELETE] = "DELETE";
    specialKeys[VK_INSERT] = "INSERT";
    specialKeys[VK_HOME] = "HOME";
    specialKeys[VK_END] = "END";
    specialKeys[VK_PRIOR] = "PAGE_UP";
    specialKeys[VK_NEXT] = "PAGE_DOWN";
    specialKeys[VK_ESCAPE] = "ESC";

    // Setas
    specialKeys[VK_LEFT] = "LEFT_ARROW";
    specialKeys[VK_RIGHT] = "RIGHT_ARROW";
    specialKeys[VK_UP] = "UP_ARROW";
    specialKeys[VK_DOWN] = "DOWN_ARROW";

    // Teclas de função
    functionKeys[VK_F1] = "F1";
    functionKeys[VK_F2] = "F2";
    functionKeys[VK_F3] = "F3";
    functionKeys[VK_F4] = "F4";
    functionKeys[VK_F5] = "F5";
    functionKeys[VK_F6] = "F6";
    functionKeys[VK_F7] = "F7";
    functionKeys[VK_F8] = "F8";
    functionKeys[VK_F9] = "F9";
    functionKeys[VK_F10] = "F10";
    functionKeys[VK_F11] = "F11";
    functionKeys[VK_F12] = "F12";

    // Modificadores
    modifierKeys.insert(VK_SHIFT);
    modifierKeys.insert(VK_CONTROL);
    modifierKeys.insert(VK_MENU); // Alt
    modifierKeys.insert(VK_LWIN);
    modifierKeys.insert(VK_RWIN);
}

KeyEvent KeyProcessor::processKey(int virtualKey) {
    KeyEvent event;
    event.virtualKey = virtualKey;
    event.timestamp = GetTickCount();
    event.isSpecial = false;
    event.hasModifier = false;

    // Verificar modificadores
    event.hasModifier = isShiftPressed() || isCtrlPressed() || isAltPressed();

    // Processar teclas especiais
    if (specialKeys.find(virtualKey) != specialKeys.end()) {
        event.keyName = specialKeys[virtualKey];
        event.isSpecial = true;

        // Casos especiais que produzem caracteres
        if (virtualKey == VK_SPACE) {
            event.actualChar = " ";
        } else if (virtualKey == VK_RETURN) {
            event.actualChar = "\n";
        } else if (virtualKey == VK_TAB) {
            event.actualChar = "\t";
        } else {
            event.actualChar = "[" + event.keyName + "]";
        }
    }
    // Teclas de função
    else if (functionKeys.find(virtualKey) != functionKeys.end()) {
        event.keyName = functionKeys[virtualKey];
        event.actualChar = "[" + event.keyName + "]";
        event.isSpecial = true;
    }
    // Letras
    else if (virtualKey >= 'A' && virtualKey <= 'Z') {
        bool capsOn = isCapsLockOn();
        bool shiftPressed = isShiftPressed();

        // XOR: maiúscula se (Caps XOR Shift)
        bool uppercase = capsOn ^ shiftPressed;

        char letter = uppercase ? virtualKey : (virtualKey + 32);
        event.actualChar = std::string(1, letter);
        event.keyName = "LETTER_" + std::string(1, virtualKey);
    }
    // Números
    else if (virtualKey >= '0' && virtualKey <= '9') {
        if (isShiftPressed()) {
            event.actualChar = getShiftedNumber(virtualKey);
            event.keyName = "SYMBOL";
        } else {
            event.actualChar = std::string(1, virtualKey);
            event.keyName = "NUMBER";
        }
    }
    // Símbolos e pontuação
    else {
        std::string symbol = getShiftedSymbol(virtualKey);
        if (!symbol.empty()) {
            event.actualChar = symbol;
            event.keyName = "SYMBOL";
        } else {
            event.actualChar = "";
            event.keyName = "UNKNOWN_" + std::to_string(virtualKey);
        }
    }

    return event;
}

std::string KeyProcessor::getShiftedNumber(char num) {
    switch (num) {
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
        default: return std::string(1, num);
    }
}

std::string KeyProcessor::getShiftedSymbol(int key) {
    if (isShiftPressed()) {
        switch (key) {
            case VK_OEM_MINUS: return "_";      // -
            case VK_OEM_PLUS: return "+";       // =
            case VK_OEM_4: return "{";          // [
            case VK_OEM_6: return "}";          // ]
            case VK_OEM_5: return "|";          // backslash
            case VK_OEM_1: return ":";          // ;
            case VK_OEM_7: return "\"";         // '
            case VK_OEM_COMMA: return "<";      // ,
            case VK_OEM_PERIOD: return ">";     // .
            case VK_OEM_2: return "?";          // /
            case VK_OEM_3: return "~";          // `
        }
    } else {
        switch (key) {
            case VK_OEM_MINUS: return "-";
            case VK_OEM_PLUS: return "=";
            case VK_OEM_4: return "[";
            case VK_OEM_6: return "]";
            case VK_OEM_5: return "\\";
            case VK_OEM_1: return ";";
            case VK_OEM_7: return "'";
            case VK_OEM_COMMA: return ",";
            case VK_OEM_PERIOD: return ".";
            case VK_OEM_2: return "/";
            case VK_OEM_3: return "`";
        }
    }
    return "";
}

bool KeyProcessor::isCapsLockOn() {
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

bool KeyProcessor::isShiftPressed() {
    return (GetKeyState(VK_SHIFT) & 0x8000) != 0;
}

bool KeyProcessor::isCtrlPressed() {
    return (GetKeyState(VK_CONTROL) & 0x8000) != 0;
}

bool KeyProcessor::isAltPressed() {
    return (GetKeyState(VK_MENU) & 0x8000) != 0;
}

bool KeyProcessor::isAlphaNumeric(int key) {
    return (key >= 'A' && key <= 'Z') || (key >= '0' && key <= '9');
}

bool KeyProcessor::isSpecialKey(int key) {
    return specialKeys.find(key) != specialKeys.end();
}

bool KeyProcessor::isFunctionKey(int key) {
    return functionKeys.find(key) != functionKeys.end();
}

bool KeyProcessor::isNavigationKey(int key) {
    return key == VK_LEFT || key == VK_RIGHT || key == VK_UP || key == VK_DOWN ||
           key == VK_HOME || key == VK_END || key == VK_PRIOR || key == VK_NEXT;
}