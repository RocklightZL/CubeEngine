#pragma once
#include "TextureRegion.h"
#include "Cube/Resource/Resource.h"

#include <glm/vec2.hpp>

namespace Cube {
    class Texture2D;

    struct Glyph {
        glm::vec2 size;
		glm::vec2 bearing; // offset of the top-left corner relative to the origin
		TextureRegion texRegion; // top-left and bottom-right
		int advance;
		Texture2D* texture;
    };

	// Font class. based on FreeType
	class Font : public ResourceBase{
	public:
		Font(const std::string& filePath, int fontSize);

		Glyph* getGlyph(uint32_t c);
	private:
		std::string filePath;
        int fontSize;
		std::unordered_map<uint32_t, Glyph> glyphs;
		std::vector<std::unique_ptr<Texture2D>> atlasPages;

		struct FreeTypeData;
		std::unique_ptr<FreeTypeData> ftData;

		Glyph* loadGlyph(uint32_t c);
		// for function loadGlyph
		int curSize = 512;
		int x = 0;
		int y = 0;
		int maxH = 0;
	};

	constexpr int MAX_TEX_SIZE = 4096;
}