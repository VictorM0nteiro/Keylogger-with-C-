// ============================================================================
// SecurityManager.h - Segurança e criptografia
// ============================================================================
#ifndef SECURITYMANAGER_H
#define SECURITYMANAGER_H

#include <string>
#include <vector>

class SecurityManager {
private:
    std::string encryptionKey;
    bool encryptionEnabled;
    std::vector<std::string> sensitivePatterns;

    std::string simpleXOR(const std::string& data, const std::string& key);
    bool containsSensitiveData(const std::string& data);
    std::string maskSensitiveData(const std::string& data);

public:
    SecurityManager();

    std::string encrypt(const std::string& data);
    std::string decrypt(const std::string& encryptedData);

    void setEncryptionKey(const std::string& key);
    void enableEncryption(bool enable) { encryptionEnabled = enable; }
    bool isEncryptionEnabled() const { return encryptionEnabled; }

    // Filtros de segurança
    std::string filterSensitiveData(const std::string& data);
    void addSensitivePattern(const std::string& pattern);
    void removeSensitivePattern(const std::string& pattern);

    // Validações
    bool isSecureEnvironment();
    bool hasAdminPrivileges();
};

#endif