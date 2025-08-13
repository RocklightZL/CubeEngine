#include "misc.h"

#include "Cube/Core/Log.h"
#include "Cube/Utils/Utils.h"

std::string getUserConfigDir() {
#ifdef _WIN32
	std::string dir = std::string(std::getenv("APPDATA"));
	if(dir.empty()) {
	    CB_EDITOR_ERROR("getUserConfigDir: Failed to get environment variable \"APPDATA\"");
	}
	return dir;
#elif __APPLE__ // TODO: øÁ∆ΩÃ®  ≈‰
	std::string dir = std::string(std::getenv("HOME"));
	if(dir.empty()) {
		CB_EDITOR_ERROR("getUserConfigDir: Failed to get environment variable \"HOME\"");
	}
	return dir + "/Liabrary/Application Support";
#elif __Linux__
	std::string dir = std::string(std::getenv("XDG_CONFIG_HOME"));
	if(!dir.empty()) {
		return dir;
	}else {
	    dir = std::getenv("HOME");
		if(dir.empty()) {
		    CB_EDITOR_ERROR("getUserConfigDir: Failed to get environment variable \"HOME\"");
		}
		return dir + "/.config";
	}
#else
	CB_EDITOR_ERROR("Unknown operating system")
	return "";
#endif
}
