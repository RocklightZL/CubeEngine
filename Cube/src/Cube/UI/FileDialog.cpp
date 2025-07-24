#include "pch.h"
#include "FileDialog.h"

#include <commctrl.h>
#include <commdlg.h>

namespace Cube {

    std::string FileDialog::openFile(const char* filter, HWND owner) {
        OPENFILENAMEA ofn;
        char filePath[260] = {};

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = owner;
        ofn.lpstrFile = filePath;
        ofn.nMaxFile = sizeof(filePath);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if(GetOpenFileNameA(&ofn)) {
            return filePath;
        }
        return "";
    }

    std::string FileDialog::saveFile(const char* filter, HWND owner) {
        OPENFILENAMEA ofn;
        char filePath[260] = {};

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = owner;
        ofn.lpstrFile = filePath;
        ofn.nMaxFile = sizeof(filePath);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
        if(GetOpenFileNameA(&ofn)) {
            return filePath;
        }
        return "";
    }
}  // namespace Cube