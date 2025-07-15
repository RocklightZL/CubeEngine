#pragma once

#include <string>

namespace Cube {

    class ResourceBase {
    public:
        ResourceBase() = default;
        virtual ~ResourceBase() = default;

        std::string path;
        int refCount = 0;
    };

    template<typename T>
    class Resource : public ResourceBase{
    public:
        T* data = nullptr;
        virtual ~Resource() {
            delete data;
        }
    };
}