#pragma once

#include "Cube/Renderer/TextureRegion.h"

#include <string>
#include <vector>

namespace Cube {

	class TextureMetadata {
    public:
        TextureMetadata(const glm::vec2& size);
        TextureMetadata(const std::string& filePath);
        ~TextureMetadata() = default;

        void toJson(const std::string& filePath) const;
        void addRegion(const glm::vec2& pos, const glm::vec2& size);
        TextureRegion getRegion(const glm::vec2& pos) const;
    private:
        glm::vec2 size;
        std::vector<TextureRegion> regions;
    };

}  // namespace Cube