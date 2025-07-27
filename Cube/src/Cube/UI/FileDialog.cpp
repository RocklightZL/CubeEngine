#include "pch.h"
#include "FileDialog.h"

#include"Cube/Utils/Utils.h"

#include <commctrl.h>
#include <commdlg.h>
#include <ShlObj.h>

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
            std::string res(filePath);
            Utils::normalizePath(res);
            return res;
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
            std::string res(filePath);
            Utils::normalizePath(res);
            return res;
        }
        return "";
    }

    std::string FileDialog::selectDir(HWND owner) {
        BROWSEINFOA bi;
        char path[260] = {};

        ZeroMemory(&bi, sizeof(bi));
        bi.hwndOwner = owner;
        bi.pszDisplayName = path;
        bi.ulFlags = BIF_NEWDIALOGSTYLE| BIF_EDITBOX;

        LPITEMIDLIST idl = SHBrowseForFolderA(&bi);
        if (idl != NULL && SHGetPathFromIDListA(idl, path)) {
            std::string res(path);
            Utils::normalizePath(res);
            return res;
        }
        return "";
    }
    
}  // namespace Cube