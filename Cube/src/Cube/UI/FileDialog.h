#pragma once

#include <string>
#include <windows.h>

namespace Cube {

    class FileDialog {
    public:
        static std::string openFile(const char* filter = "All files\0*.*\0", HWND owner = nullptr);
        static std::string saveFile(const char* filter = "All files\0*.*\0", HWND owner = nullptr);
        static std::string selectDir(HWND owner = nullptr);
        static std::vector<std::string> openMultiFiles(const char* filter = "All files\0*.*\0", HWND owner = nullptr);
    };

}