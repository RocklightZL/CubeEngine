#pragma once

#include <string>

namespace Cube {

    class ResourceBase {
    public:
        friend class ResourceManager;

        ResourceBase() = default;
        virtual ~ResourceBase() = default;

        std::string getIdentifier() const { return identifier; }

    protected:
        std::string identifier;

    private:
        int refCount = 0;
    };
}