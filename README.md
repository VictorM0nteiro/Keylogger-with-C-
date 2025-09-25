# Keylogger Robust (incomplete)

A robust keylogger project written in C++ for educational and cybersecurity research purposes. This application demonstrates keylogging, file management, encryption, and advanced configuration features in a modular and extensible way.

## Features
- Start/stop keylogging
- Output file configuration
- Optional encryption of logs
- Log viewing and cleaning
- Hidden mode toggle
- Advanced settings (auto-save interval, max file size, security filters)

## Folder Structure
```
include/
    ConfigManager.h
    FileManager.h
    KeyLogger.h
    KeyProcessor.h
    SecurityManager.h
    Utils.h
src/
    KeyLogger.cpp
    main.cpp
```

## How to Build
1. Make sure you have a C++ compiler installed (e.g., g++, MSVC).
2. Compile the project:
   - On Windows (cmd):
     ```
     g++ src\main.cpp src\KeyLogger.cpp -Iinclude -o keylogger.exe
     ```
   - On Linux:
     ```
     g++ src/main.cpp src/KeyLogger.cpp -Iinclude -o keylogger
     ```

## How to Run
- Run the executable:
  - On Windows:
    ```
    keylogger.exe
    ```
  - On Linux:
    ```
    ./keylogger
    ```

## Usage
Follow the menu in the terminal to:
- Start/stop logging
- Change output file
- Enable/disable encryption
- View or clean logs
- Toggle hidden mode
- Access advanced settings

## Disclaimer
This project is for educational purposes only. Do not use it for unauthorized monitoring or malicious activities. Always comply with local laws and obtain proper consent before using keylogging software.

## License
MIT License

