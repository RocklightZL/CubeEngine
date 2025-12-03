#pragma once
#include "Resource.h"
#include "ResourceManager.h"

namespace Cube {

    template<typename T>
    class ResPtr {
        static_assert(std::is_base_of_v<ResourceBase, T> && "Only supports resource types!");
    public:
        ResPtr() = default;
        ResPtr(std::nullptr_t) : resource(nullptr){}
        explicit ResPtr(const std::string& path) {
            resource = ResourceManager::get().load<T>(path);
        }
        ResPtr(const ResPtr& other) : ResPtr(static_cast<ResourceBase*>(other.get())->getPath()){}
        ResPtr(ResPtr&& other) noexcept : resource(other.resource){
            other.resource = nullptr;
        }
        ~ResPtr() {
            if(resource) {
                ResourceManager::get().release(resource);
            }
        }

        ResPtr& operator=(const ResPtr& other) {
            if(&other == this) return *this;
            if(resource) {
                ResourceManager::get().release(resource);
            }
            resource = ResourceManager::get().load<T>(static_cast<ResourceBase*>(other.get())->getPath());
            return *this;
        }
        ResPtr& operator=(ResPtr&& other) noexcept {
            if(&other == this) return *this;
            if(resource) {
                ResourceManager::get().release(resource);
            }
            resource = other.resource;
            other.resource = nullptr;
            return *this;
        }
        ResPtr& operator=(std::nullptr_t) noexcept {
            reset();
            return *this;
        }

        T* operator->() const {
            return resource;
        }

        T& operator*() const {
            CB_ASSERT(resource && "resource is empty!");
            return *resource;
        }

        explicit operator bool() const noexcept {
            return resource != nullptr;
        }

        void reset() noexcept {
            if(resource) {
                ResourceManager::get().release(resource);
                resource = nullptr;
            }
        }
        void reset(const std::string& path) {
            reset();
            resource = ResourceManager::get().load<T>(path);
        }

        T* get() const {
            return resource;
        }

        bool isNull() const {
            return resource == nullptr;
        }

    private:
        T* resource = nullptr;
    };

}