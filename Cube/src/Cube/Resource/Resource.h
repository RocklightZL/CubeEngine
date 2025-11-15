#pragma once

namespace Cube {

    class ResourceBase {
    public:
        friend class ResourceManager;

        // Derived classes must implement a constructor with a std::string type parameter.
        explicit ResourceBase(const std::string& path) : path(path){}
        ResourceBase() = default;
        virtual ~ResourceBase() = default;

        std::string getPath() const { return path; }

    protected:
        std::string path; // resource file path
        int refCount = 0;
    };
}