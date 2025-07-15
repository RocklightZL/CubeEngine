#include "pch.h"
#include "ResourceManager.h"

namespace Cube {

    ResourceManager& ResourceManager::getInstance() {
		static ResourceManager instance;
		return instance;
	}
}  // namespace Cube