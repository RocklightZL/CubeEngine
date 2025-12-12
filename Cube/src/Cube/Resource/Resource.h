#pragma once

#include <string>

#include "AssetMeta.h"
#include "RUID.h"

namespace Cube {

    class ResourceBase {
    public:
        friend class ResourceManager;

        // Derived classes must implement a constructor with a AssetMeta type parameter.
        explicit ResourceBase(const AssetMeta& metadata) : ruid(metadata.ruid){}
        ResourceBase() = default;
        virtual ~ResourceBase() = default;

        RUID getRuid() const { return ruid; }
        ResourceType getType() const {
            return getResourceType(ruid);
        }

    protected:
        RUID ruid = 0;

    private:
        int refCount = 0;
    };
}