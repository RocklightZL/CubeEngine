#pragma once

#include <string>

namespace Cube {

	class Utils {
	public:
		Utils() = delete;
		~Utils() = delete;

        static std::string readFileToString(const std::string& filePath);
		static void copyFile(const std::string& srcPath, const std::string& destPath);
		static bool isFileInDirectory(const std::string& file, const std::string& directory);
		static void normalizePath(std::string& path);
		static std::string getFileName(const std::string& path, bool keepSuffix = false);
		static std::string getFileSuffix(const std::string& path);
	};

}