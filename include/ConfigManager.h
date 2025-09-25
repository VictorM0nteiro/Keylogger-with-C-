// ============================================================================
// ConfigManager.h - Gerenciamento de configurações
// ============================================================================
#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <string>
#include <map>

struct LoggerConfig {
    std::string outputFile;
    int autoSaveInterval;
    size_t maxFileSize;
    bool encryptionEnabled;
    bool hiddenMode;
    bool timestampEnabled;
    bool backupEnabled;
    std::string encryptionKey;
    bool filterSensitiveData;
    int maxLogFiles;
};

class ConfigManager {
private:
    LoggerConfig config;
    std::string configFile;

    void setDefaults();
    bool parseConfigFile();
    std::string trim(const std::string& str);

public:
    ConfigManager(const std::string& configPath = "config.ini");

    bool loadConfig();
    bool saveConfig();

    // Getters
    const LoggerConfig& getConfig() const { return config; }
    std::string getOutputFile() const { return config.outputFile; }
    int getAutoSaveInterval() const { return config.autoSaveInterval; }
    size_t getMaxFileSize() const { return config.maxFileSize; }
    bool isEncryptionEnabled() const { return config.encryptionEnabled; }
    bool isHiddenMode() const { return config.hiddenMode; }

    // Setters
    void setOutputFile(const std::string& file) { config.outputFile = file; }
    void setAutoSaveInterval(int seconds) { config.autoSaveInterval = seconds; }
    void setMaxFileSize(size_t size) { config.maxFileSize = size; }
    void setEncryption(bool enable) { config.encryptionEnabled = enable; }
    void setHiddenMode(bool hidden) { config.hiddenMode = hidden; }
};

#endif
