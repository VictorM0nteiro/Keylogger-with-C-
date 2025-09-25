// ============================================================================
// Utils.h - Utilitários gerais
// ============================================================================
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <chrono>

namespace Utils {
    // Formatação de tempo
    std::string getCurrentTimestamp();
    std::string formatDuration(const std::chrono::seconds& duration);

    // Manipulação de strings
    std::string toUpper(const std::string& str);
    std::string toLower(const std::string& str);
    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string join(const std::vector<std::string>& vec, const std::string& delimiter);
    std::string trim(const std::string& str);

    // Sistema
    bool isRunningAsAdmin();
    std::string getComputerName();
    std::string getUserName();
    bool fileExists(const std::string& filename);
    size_t getFileSize(const std::string& filename);

    // Criptografia simples
    std::string base64Encode(const std::string& data);
    std::string base64Decode(const std::string& encoded);
    std::string generateRandomKey(int length);

    // Validação
    bool isValidFilename(const std::string& filename);
    bool isSecurePassword(const std::string& password);
}

#endif
