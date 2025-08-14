#pragma once

#include <string>
#include <unordered_map>

namespace Cube {

	class IniReader {
	public:
		IniReader(const std::string& filePath);
		~IniReader() = default;
	private:
	};

}