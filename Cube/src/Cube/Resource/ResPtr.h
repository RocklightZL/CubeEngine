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
        explicit ResPtr(RUID ruid) {
            resource = ResourceManager::get().load<T>(ruid);
        }
        ResPtr(const ResPtr& other) : resource(nullptr){
            if(other.resource) {
                resource = ResourceManager::get().load<T>(static_cast<ResourceBase*>(other.get())->getRuid());
            }
        }
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
            resource = ResourceManager::get().load<T>(static_cast<ResourceBase*>(other.get())->getRuid());
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
        void reset(RUID ruid) {
            reset();
            resource = ResourceManager::get().load<T>(ruid);
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