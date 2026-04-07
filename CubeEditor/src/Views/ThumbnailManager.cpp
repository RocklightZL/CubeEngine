#include "ThumbnailManager.h"

#include "Cube/Core/Log.h"
#include "Cube/Renderer/Texture.h"

#include <stb_image.h>
#include <stb_image_resize2.h>

ThumbnailManager::ThumbnailManager() {
    worker = std::thread(&ThumbnailManager::workerLoop, this);
}

ThumbnailManager::ThumbnailManager(const Config& config) : config(config) {
    worker = std::thread(&ThumbnailManager::workerLoop, this);
}

ThumbnailManager::~ThumbnailManager() {
    running = false;
    pendingCv.notify_all();
    if(worker.joinable()) worker.join();
}

Cube::Texture2D* ThumbnailManager::request(const std::string& path) {
    if(path.empty()) return nullptr;

    auto it = cache.find(path);
    if(it == cache.end()) {
        CacheEntry e;
        e.state = State::Missing;
        e.path = path;

        lru.push_front(path);
        e.lruIt = lru.begin();

        auto [iterator, ok] = cache.emplace(path, std::move(e));
        CB_ASSERT(ok);
        it = iterator;
    }
    CacheEntry& entry = it->second;
    entry.path = path;
    touchLRU(&entry);
    if(entry.state == State::Ready) return entry.texture ? entry.texture.get() : nullptr;
    if(entry.state == State::Pending) return nullptr;
    if(entry.state == State::Failed) {
        return nullptr;
    }

    entry.state = State::Pending;
    {
        std::lock_guard<std::mutex> lock(pendingMutex);
        if((int)pendingTasks.size() < config.decodeQueueLimit) {
            pendingTasks.push_back({path});
        } else {
            entry.state = State::Missing;
            return nullptr;
        }
    }
    pendingCv.notify_one();
    return nullptr;
}

void ThumbnailManager::tick() {
    int uploadCnt = 0;
    while(uploadCnt < config.uploadPerFrame) {
        CompletedTask task;
        {
            std::lock_guard<std::mutex> lock(completedMutex);
            if(completedTasks.empty()) break;
            task = std::move(completedTasks.front());
            completedTasks.pop_front();
        }
        auto it = cache.find(task.path);
        if(it == cache.end()) continue;
        CacheEntry& e = it->second;
        if(!task.ok || task.rgba.size() != config.thumbSize * config.thumbSize * 4) {
            e.state = State::Failed;
            e.texture.reset();
            continue;
        }
        e.texture = std::make_unique<Cube::Texture2D>(config.thumbSize, config.thumbSize, task.rgba.data());
        e.state = State::Ready;
        touchLRU(&e);
        ++uploadCnt;
    }
}

void ThumbnailManager::clear() {
    cache.clear();
    lru.clear();
    {
        std::lock_guard<std::mutex> lk(pendingMutex);
        pendingTasks.clear();
    }
    {
        std::lock_guard<std::mutex> lk(completedMutex);
        completedTasks.clear();
    }
}

void ThumbnailManager::invalidate(const std::string& path) {
    auto it = cache.find(path);
    if(it == cache.end()) return;
    lru.erase(it->second.lruIt);
    cache.erase(it);
}

void ThumbnailManager::workerLoop() {
    while(running) {
        PendingTask task;
        {
            std::unique_lock<std::mutex> lock(pendingMutex);
            pendingCv.wait(lock, [this]{ return !pendingTasks.empty() || !running; });
            if(!running) break;
            task = std::move(pendingTasks.front());
            pendingTasks.pop_front();
        }
        CompletedTask out;
        out.path = task.path;
        out.ok = buildThumbnailRGBA(task.path, config.thumbSize, true, &out.rgba);
        {
            std::lock_guard<std::mutex> doneLock(completedMutex);
            completedTasks.push_back(std::move(out));
        }
    }
}

void ThumbnailManager::touchLRU(CacheEntry* cacheEntry) {
    lru.erase(cacheEntry->lruIt);
    lru.push_front(cacheEntry->path);
    cacheEntry->lruIt = lru.begin();
}

bool ThumbnailManager::buildThumbnailRGBA(const std::string& path, int thumbSize, bool keepAspect, std::vector<uint8_t>* outRGBA) {
    stbi_set_flip_vertically_on_load(1);
    int w, h, ch;
    uint8_t* src = stbi_load(path.c_str(), &w, &h, &ch, 4);
    if(!src) {
        CB_EDITOR_ERROR("Thumbnail decode failed: {}", path);
        return false;
    }

    outRGBA->resize(thumbSize * thumbSize * 4);
    std::memset(outRGBA->data(), 0, thumbSize * thumbSize * 4);
    if(keepAspect) {
        float sx = (float)thumbSize / (float)w;
        float sy = (float)thumbSize / (float)h;
        float s = std::min(sx, sy);
        int newW = std::max(1, (int)(w * s));
        int newH = std::max(1, (int)(h * s));

        uint8_t* scaled = new uint8_t[newW * newH * 4];
        stbir_resize_uint8_linear(src, w, h, 0, scaled, newW, newH, 0, STBIR_RGBA);
        int offsetX = (thumbSize - newW) / 2;
        int offsetY = (thumbSize - newH) / 2;
        for(int y = 0; y < newH; ++y) {
            uint8_t* dstRow = outRGBA->data() + ((y + offsetY) * thumbSize + offsetX) * 4;
            const uint8_t* srcRow = scaled + (y * newW * 4);
            std::memcpy(dstRow, srcRow, newW * 4);
        }
        delete[] scaled;
    }else {
        stbir_resize_uint8_linear(src, w, h, 0, outRGBA->data(), thumbSize, thumbSize, 0, STBIR_RGBA);
    }

    stbi_image_free(src);
    return true;
}
