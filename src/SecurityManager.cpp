// ============================================================================
// SecurityManager.cpp - Implementação completa
// ============================================================================
#include "SecurityManager.h"
#include "Utils.h"
#include <algorithm>
#include <regex>
#include <iostream>
#include <mutex>
#include <thread>


SecurityManager::SecurityManager() : encryptionEnabled(false) {
    encryptionKey = "DefaultKey123!@#";

    // Padrões sensíveis padrão
    sensitivePatterns = {
        "password", "senha", "pass", "pwd",
        "login", "user", "email", "mail",
        "credit", "card", "cvv", "ssn",
        "pin", "code", "token", "key"
    };
}

std::string SecurityManager::encrypt(const std::string& data) {
    if (!encryptionEnabled || data.empty()) {
        return data;
    }

    return Utils::base64Encode(simpleXOR(data, encryptionKey));
}

std::string SecurityManager::decrypt(const std::string& encryptedData) {
    if (!encryptionEnabled || encryptedData.empty()) {
        return encryptedData;
    }

    try {
        std::string decoded = Utils::base64Decode(encryptedData);
        return simpleXOR(decoded, encryptionKey);
    } catch (const std::exception&) {
        return "[ERRO_DECODIFICACAO]";
    }
}

std::string SecurityManager::simpleXOR(const std::string& data, const std::string& key) {
    std::string result = data;
    size_t keyLen = key.length();

    for (size_t i = 0; i < result.length(); i++) {
        result[i] ^= key[i % keyLen];
    }

    return result;
}

bool SecurityManager::containsSensitiveData(const std::string& data) {
    std::string lowerData = Utils::toLower(data);

    for (const auto& pattern : sensitivePatterns) {
        if (lowerData.find(Utils::toLower(pattern)) != std::string::npos) {
            return true;
        }
    }

    // Verificar padrões regex para email, números de cartão, etc.
    std::regex emailPattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    std::regex cardPattern(R"(\b\d{4}[\s-]?\d{4}[\s-]?\d{4}[\s-]?\d{4}\b)");

    return std::regex_search(data, emailPattern) || std::regex_search(data, cardPattern);
}

std::string SecurityManager::maskSensitiveData(const std::string& data) {
    std::string result = data;

    // Mascarar emails
    std::regex emailPattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    result = std::regex_replace(result, emailPattern, "[EMAIL_MASCARADO]");

    // Mascarar números de cartão
    std::regex cardPattern(R"(\b\d{4}[\s-]?\d{4}[\s-]?\d{4}[\s-]?\d{4}\b)");
    result = std::regex_replace(result, cardPattern, "[CARTAO_MASCARADO]");

    // Mascarar sequências que parecem senhas
    std::regex passPattern(R"(pass\w*[:=]\s*\S+)", std::regex_constants::icase);
    result = std::regex_replace(result, passPattern, "[SENHA_MASCARADA]");

    return result;
}

std::string SecurityManager::filterSensitiveData(const std::string& data) {
    if (containsSensitiveData(data)) {
        return maskSensitiveData(data);
    }
    return data;
}

void SecurityManager::setEncryptionKey(const std::string& key) {
    if (key.length() >= 8) { // Mínimo 8 caracteres
        encryptionKey = key;
    } else {
        std::cerr << "Aviso: Chave muito curta, usando padrão" << std::endl;
    }
}

void SecurityManager::addSensitivePattern(const std::string& pattern) {
    if (std::find(sensitivePatterns.begin(), sensitivePatterns.end(), pattern)
        == sensitivePatterns.end()) {
        sensitivePatterns.push_back(pattern);
    }
}

void SecurityManager::removeSensitivePattern(const std::string& pattern) {
    auto it = std::find(sensitivePatterns.begin(), sensitivePatterns.end(), pattern);
    if (it != sensitivePatterns.end()) {
        sensitivePatterns.erase(it);
    }
}

bool SecurityManager::isSecureEnvironment() {
    // Verificações básicas de segurança
    return hasAdminPrivileges() && Utils::isRunningAsAdmin();
}

bool SecurityManager::hasAdminPrivileges() {
    return Utils::isRunningAsAdmin();
}