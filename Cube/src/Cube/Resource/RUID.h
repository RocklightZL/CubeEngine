#pragma once

#include <random>

#include "ResourceType.h"

namespace Cube {

	using RUID = uint64_t;

	class RUIDGenerator {
	public:
		static RUID gen(ResourceType type);

	private:
		thread_local static std::mt19937_64 randomEngine;
		thread_local static std::uniform_int_distribution<uint64_t> distribution;
	};

	inline ResourceType getResourceType(RUID ruid) {
		return static_cast<ResourceType>((ruid >> 56) & 0xFF);
    }

}