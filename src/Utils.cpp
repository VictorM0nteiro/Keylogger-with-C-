// ============================================================================
// Utils.cpp - Implementação completa
// ============================================================================
#include "../include/Utils.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <windows.h>
#include <lmcons.h>
#include <random>
#include <mutex>
#include <thread>


namespace Utils {

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string formatDuration(const std::chrono::seconds& duration) {
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration % std::chrono::hours(1));
    auto seconds = duration % std::chrono::minutes(1);
    
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << hours.count() << ":"
       << std::setfill('0') << std::setw(2) << minutes.count() << ":"
       << std::setfill('0') << std::setw(2) << seconds.count();
    
    return ss.str();
}

std::string toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

std::string join(const std::vector<std::string>& vec, const std::string& delimiter) {
    if (vec.empty()) return "";
    
    std::string result = vec[0];
    for (size_t i = 1; i < vec.size(); ++i) {
        result += delimiter + vec[i];
    }
    
    return result;
}

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

bool isRunningAsAdmin() {
    BOOL isAdmin = FALSE;
    PSID administratorsGroup = nullptr;
    
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    
    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
                                DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
                                &administratorsGroup)) {
        CheckTokenMembership(nullptr, administratorsGroup, &isAdmin);
        FreeSid(administratorsGroup);
    }
    
    return isAdmin != FALSE;
}

std::string getComputerName() {
    char buffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(buffer);
    
    if (GetComputerNameA(buffer, &size)) {
        return std::string(buffer);
    }
    
    return "UNKNOWN";
}

std::string getUserName() {
    char buffer[UNLEN + 1];
    DWORD size = sizeof(buffer);
    
    if (GetUserNameA(buffer, &size)) {
        return std::string(buffer);
    }
    
    return "UNKNOWN";
}

bool fileExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}

size_t getFileSize(const std::string& filename) {
    try {
        if (fileExists(filename)) {
            return std::filesystem::file_size(filename);
        }
    } catch (const std::exception&) {
        return 0;
    }
    return 0;
}

std::string base64Encode(const std::string& data) {
    static const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    int val = 0, valb = -6;
    
    for (unsigned char c : data) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            result.push_back(chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    
    if (valb > -6) {
        result.push_back(chars[((val << 8) >> (valb + 8)) & 0x3F]);
    }
    
    while (result.size() % 4) {
        result.push_back('=');
    }
    
    return result;
}

std::string base64Decode(const std::string& encoded) {
    static const int decodeTable[128] = {
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
        52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-2,-1,-1,
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
        15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
        -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
        41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1
    };
    
    std::string result;
    int val = 0, valb = -8;
    
    for (char c : encoded) {
        if (decodeTable[c] == -1) break;
        if (decodeTable[c] == -2) break; // padding
        
        val = (val << 6) + decodeTable[c];
        valb += 6;
        if (valb >= 0) {
            result.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    
    return result;
}

std::string generateRandomKey(int length) {
    const std::string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, charset.size() - 1);
    
    std::string key;
    for (int i = 0; i < length; ++i) {
        key += charset[dis(gen)];
    }
    
    return key;
}

bool isValidFilename(const std::string& filename) {
    if (filename.empty() || filename.length() > 255) {
        return false;
    }
    
    // Caracteres proibidos no Windows
    const std::string invalidChars = "<>:\"|?*";
    for (char c : invalidChars) {
        if (filename.find(c) != std::string::npos) {
            return false;
        }
    }
    
    // Nomes reservados
    const std::vector<std::string> reservedNames = {
        "CON", "PRN", "AUX", "NUL",
        "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9",
        "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"
    };
    
    std::string upperFilename = toUpper(filename);
    for (const std::string& reserved : reservedNames) {
        if (upperFilename == reserved || upperFilename.find(reserved + ".") == 0) {
            return false;
        }
    }
    
    return true;
}

bool isSecurePassword(const std::string& password) {
    if (password.length() < 8) return false;
    
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    
    for (char c : password) {
        if (std::isupper(c)) hasUpper = true;
        else if (std::islower(c)) hasLower = true;
        else if (std::isdigit(c)) hasDigit = true;
        else hasSpecial = true;
    }
    
    return hasUpper && hasLower && hasDigit && hasSpecial;
}

} // namespace Utils