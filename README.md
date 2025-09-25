# KeyLogger Pro

Key capture system developed in C++ with modular architecture.

## 📋 About the Project

KeyLogger Pro is an educational project that demonstrates:
- System programming in C++
- Keyboard event capture
- Modular architecture with classes
- Encryption and filtering system
- File and configuration management

## 🏗️ Project Structure

```
KeyLogger-Pro/
├── include/          # Headers (.h)
├── src/             # Implementations (.cpp)
├── logs/            # Log files
├── bin/             # Executables
└── scripts/         # Build scripts
```

## 🚀 Installation & Usage

### Requirements
- Windows 10+ or Linux
- GCC 7.0+ with C++17 support
- Make (optional)

### Compilation
```bash
# Method 1: Automated script
./scripts/build.sh      # Linux/Mac
scripts/build.bat       # Windows

# Method 2: Make
make setup && make release

# Method 3: Manual
g++ -std=c++17 -Iinclude src/*.cpp -o bin/keylogger.exe -luser32 -ladvapi32
```

### Execution
```bash
./bin/keylogger
```

## ⚙️ Configuration

Edit `config/config.ini`:
```ini
[General]
OutputFile=logs/keylog.txt
AutoSaveInterval=30
MaxFileSize=10485760

[Security]
EncryptionEnabled=true
FilterSensitiveData=true
```

## 🎮 Features

### Menu Interface
```
=== KEYLOGGER PRO ===
1. Start capture
2. Configure output file
3. Enable/Disable encryption
4. View logs
5. Advanced settings
0. Exit
```

### Key Capture
- ✅ Letters (a-z, A-Z) with Caps Lock and Shift
- ✅ Numbers (0-9) and symbols
- ✅ Special keys (Enter, Tab, Backspace, etc.)
- ✅ Function keys (F1-F12)
- ✅ Arrows and navigation keys
- ✅ Combinations (Ctrl+C, Alt+Tab, etc.)

### Security
- 🔒 XOR + Base64 encryption
- 🎭 Automatic sensitive data filtering
- 📧 Email and password masking

### File Management
- 📁 Automatic rotation when file is large
- 💾 Backup system with timestamp
- 🗂️ Session history

## 📊 Main Classes

### KeyLogger
Main class that coordinates the entire system.
```cpp
KeyLogger logger;
logger.initialize();
logger.start();          // Start capture
logger.stop();           // Stop capture
```

### FileManager
Manages log files, backup, and rotation.
```cpp
FileManager fm("logs/keylog.txt");
fm.writeWithTimestamp("Message");
fm.setMaxFileSize(10 * 1024 * 1024);  // 10MB
```

### KeyProcessor
Processes keys and detects modifiers.
```cpp
KeyProcessor kp;
KeyEvent event = kp.processKey(virtualKey);
bool capsOn = kp.isCapsLockOn();
```

### SecurityManager
Encryption and sensitive data filtering.
```cpp
SecurityManager sm;
sm.setEncryptionKey("MyKey123");
std::string encrypted = sm.encrypt("data");
```

### ConfigManager
Manages settings via INI file.
```cpp
ConfigManager cm("config/config.ini");
cm.loadConfig();
int interval = cm.getAutoSaveInterval();
```

## 🔧 Usage Example

```cpp
#include "KeyLogger.h"

int main() {
    KeyLogger logger;
    
    if (!logger.initialize()) {
        return 1;
    }
    
    logger.setOutputFile("my_log.txt");
    logger.enableEncryption(true);
    logger.start();
    
    // Ctrl+Alt+Q to stop
    
    return 0;
}
```

## 📝 Log Example

```
[2025-09-25 14:30:15] === KEYLOGGER STARTED ===
[2025-09-25 14:30:22] Hello World
[2025-09-25 14:30:25] [BACKSPACE]
[2025-09-25 14:30:27] KeyLogger Pro!
[2025-09-25 14:30:32] Email: [EMAIL_MASKED]
[2025-09-25 14:30:40] === KEYLOGGER STOPPED ===
```

## 🛡️ Security

### Automatic Filtering
The system automatically detects and masks:
- Emails: `test@email.com` → `[EMAIL_MASKED]`
- Passwords: `password123` → `[PASSWORD_MASKED]`
- Credit cards: `1234 5678 9012 3456` → `[CARD_MASKED]`

### Encryption
```cpp
// Data is encrypted before saving
"Hello World" → "SGVsbG8gV29ybGQ=" (Base64 of XOR)
```

## 📋 Manual Compilation

### Windows (MinGW)
```batch
g++ -std=c++17 -Wall -O2 -Iinclude src/*.cpp -o bin/keylogger.exe -luser32 -ladvapi32
```

### Linux
```bash
g++ -std=c++17 -Wall -O2 -Iinclude src/*.cpp -o bin/keylogger -lpthread
```

### Debug
```bash
g++ -std=c++17 -g -O0 -DDEBUG -Iinclude src/*.cpp -o bin/keylogger_debug.exe -luser32 -ladvapi32
```

## ⚖️ Legal Considerations

⚠️ **IMPORTANT**: This project is for educational purposes only.

### Permitted Use
- ✅ Learning C++ programming
- ✅ Own systems
- ✅ Research and development
- ✅ Authorized security testing

### Prohibited Use
- ❌ Unauthorized monitoring
- ❌ Information theft
- ❌ Installation without consent
- ❌ Malicious use

### Responsibility
The user is fully responsible for the use of this tool. Use only ethically and legally, respecting local privacy laws.

## 📚 Additional Documentation

- `docs/API_REFERENCE.md` - Full API reference
- `docs/USAGE.md` - Detailed usage manual
- `docs/SECURITY_GUIDELINES.md` - Security guidelines

## 🤝 Contribution

1. Fork the project
2. Create a branch for your feature
3. Commit your changes
4. Submit a Pull Request

## 📄 License

This project is distributed under an educational license. See `LICENSE` for more details.
