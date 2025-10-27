#pragma once

namespace Cube {

    class ResourceBase {
    public:
        ResourceBase() = default;
        virtual ~ResourceBase() = default;

        int refCount = 0;
        std::string identifier; // format: filepath?param1&param2... eg. "C:/data/abc.ttf&24"
    };
}