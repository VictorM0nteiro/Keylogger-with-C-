// ============================================================================
// KeyProcessor.h - Processamento de teclas
// ============================================================================
#ifndef KEYPROCESSOR_H
#define KEYPROCESSOR_H

#include <string>
#include <map>
#include <set>
#include <windows.h>

struct KeyEvent {
    int virtualKey;
    std::string keyName;
    std::string actualChar;
    bool isSpecial;
    bool hasModifier;
    DWORD timestamp;
};

class KeyProcessor {
private:
    std::map<int, std::string> specialKeys;
    std::map<int, std::string> functionKeys;
    std::set<int> modifierKeys;

    void initializeKeyMaps();
    bool isModifierPressed(int modifier);
    std::string getShiftedNumber(char num);
    std::string getShiftedSymbol(int key);

public:
    KeyProcessor();

    KeyEvent processKey(int virtualKey);
    std::string virtualKeyToString(int key);
    std::string getKeyDescription(int key);

    bool isCapsLockOn();
    bool isShiftPressed();
    bool isCtrlPressed();
    bool isAltPressed();

    // Filtros
    bool isAlphaNumeric(int key);
    bool isSpecialKey(int key);
    bool isFunctionKey(int key);
    bool isNavigationKey(int key);
};

#endif