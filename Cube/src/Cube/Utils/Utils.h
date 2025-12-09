#pragma once

#include <filesystem>
#include <string>

namespace Cube {

	namespace Utils {

        std::string readFileToString(const std::string& filePath);
		void copyFile(const std::string& srcPath, const std::string& destPath);
		bool isFileInDirectory(const std::string& file, const std::string& directory);
		std::string getFileName(const std::string& path, bool keepSuffix = false);
		std::vector<uint32_t> utf8To32(const std::string& utf8_str);

        // '\\' to '/'
        inline void normalizePath(std::string& path) {
            for(int i = 0; i < path.size(); ++i) {
                if(path[i] == '\\') {
                    path[i] = '/';
                }
            }
        }

        inline std::string getFileSuffix(const std::string& path) {
            size_t pos = path.find_last_of('.');
            if(pos == std::string::npos) {
                return "";
            }
            return path.substr(pos);
        }


        inline bool isFileExists(const std::string& path) {
            return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
        }

        inline std::string getParentPath(const std::string& path) {
            size_t pos = path.find_last_of('/');
            if(pos == std::string::npos) {
                return "";
            }
            return path.substr(0, pos);
        }
	};
}