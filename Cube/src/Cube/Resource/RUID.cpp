#include "pch.h"
#include "RUID.h"

namespace Cube {
    thread_local std::mt19937_64 RUIDGenerator::randomEngine(std::random_device{}());
    thread_local std::uniform_int_distribution<uint64_t> RUIDGenerator::distribution;

    RUID RUIDGenerator::gen(ResourceType type) {
        uint64_t ruid = distribution(randomEngine);
        ruid = (ruid & 0x00FFFFFFFFFFFFFF) | (static_cast<uint64_t>(type) << 56);
        return ruid;
    }
}  // namespace Cube
