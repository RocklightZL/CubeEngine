#pragma once
#include <deque>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace Cube {
    class Texture2D;
}
class ThumbnailManager {
public:
    struct Config {
        int thumbSize = 128;
        size_t maxThumbCount = 512;
        int uploadPerFrame = 2;
        int decodeQueueLimit = 512;
    };

    ThumbnailManager();
    explicit ThumbnailManager(const Config& config);
    ~ThumbnailManager();

    ThumbnailManager(const ThumbnailManager&) = delete;
    ThumbnailManager& operator=(const ThumbnailManager&) = delete;

    Cube::Texture2D* request(const std::string& path);
    void tick();
    void clear();
    void invalidate(const std::string& path);

private:
    enum class State { Missing, Pending, Ready, Failed };

    struct CacheEntry {
        State state = State::Missing;
        std::unique_ptr<Cube::Texture2D> texture;
        std::list<std::string>::iterator lruIt;
        std::string path;
    };

    struct PendingTask {
        std::string path;
    };

    struct CompletedTask {
        std::string path;
        std::vector<uint8_t> rgba;
        bool ok = false;
    };

    Config config;

    std::unordered_map<std::string, CacheEntry> cache;
    std::list<std::string> lru;

    std::mutex pendingMutex;
    std::condition_variable pendingCv;
    std::deque<PendingTask> pendingTasks;

    std::mutex completedMutex;
    std::deque<CompletedTask> completedTasks;

    std::thread worker;
    std::atomic<bool> running = true;

    void workerLoop();
    void touchLRU(CacheEntry* cacheEntry);

    static bool buildThumbnailRGBA(const std::string& path, int thumbSize, bool keepAspect, std::vector<uint8_t>* outRGBA);
};
