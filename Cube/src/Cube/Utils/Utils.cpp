#include "pch.h"
#include "Utils.h"

#include <fstream>
#include <sstream>

namespace Cube {

    std::string Utils::readFileToString(const std::string& filePath) {
        std::ifstream file(filePath);
        if(!file.is_open()) {
            CB_CORE_ERROR("cannot open file " + filePath);
            CB_ASSERT("Failed to open file!");
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    void Utils::copyFile(const std::string& srcPath, const std::string& destPath) {
        std::ifstream srcFile(srcPath, std::ios::binary);
        if(!srcFile.is_open()) {
            CB_CORE_ERROR("Utils::copyFile: Failed to open source file: {}", srcPath);
            return;
        }

        std::ofstream destFile(destPath, std::ios::binary);
        if(!destFile.is_open()) {
            CB_CORE_ERROR("Utils::copyFile: Failed to open destination file: {}", destPath);
            return;
        }

        destFile << srcFile.rdbuf();

        if(srcFile.fail() || destFile.fail()) {
            CB_CORE_ERROR("Utils::copyFile: Failed to copy file from {} to {}", srcPath, destPath);
        }
    }

    // please pass path with '/' as separator. case-sensitive
    bool Utils::isFileInDirectory(const std::string& file, const std::string& directory) {
        if(directory.empty()) {
            CB_CORE_ERROR("Utils::isFileInDirectory: directory cannot be empty");
            return false;
        }
        if(file.size() <= directory.size()) return false;
        for(int i = 0; i < directory.size(); i++) {
            if(file[i] != directory[i]) {
                return false;
            }
        }
        if(directory.back() != '/' && file[directory.size()] != '/') return false;
        return true;
    }

    // '\\' to '/'
    void Utils::normalizePath(std::string& path) {
        for(int i = 0; i < path.size(); ++i) {
            if(path[i] == '\\') {
                path[i] = '/';
            }
        }
    }

    // get file name from path
    std::string Utils::getFileName(const std::string& path, bool keepSuffix) {
        size_t begin = path.find_last_of('/');
        size_t end = path.find_last_of('.');
        if(begin == std::string::npos) {
            CB_CORE_ERROR("Utils::getFileName: invalid path");
            return "";
        }
        if(end == std::string::npos) {
            CB_CORE_WARN("Utils::getFileName: No suffix. This may be a directory.");
            return path.substr(begin + 1);
        }
        if(keepSuffix) {
            return path.substr(begin + 1);
        }else {
            return path.substr(begin + 1, end - begin - 1);
        }
    }
}  // namespace Cube