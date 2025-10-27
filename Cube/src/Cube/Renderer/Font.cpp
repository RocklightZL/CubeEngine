#include "pch.h"
#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <algorithm>
#include "Texture.h"

namespace Cube {

    struct Font::FreeTypeData {
        FT_Library lib;
        FT_Face face;
        ~FreeTypeData() {
            if(face) FT_Done_Face(face);
            if(lib) FT_Done_FreeType(lib);
        }
    };

    Font::Font(const std::string& filePath, int fontSize) : filePath(filePath), fontSize(fontSize) {
        atlasPages.push_back(nullptr);
        ftData = std::make_unique<FreeTypeData>();
        if(FT_Error err = FT_Init_FreeType(&ftData->lib)) {
            CB_CORE_ERROR("FreeType: Failed to initialize library. Error Code = {}", err);
            return;
        }

        if(FT_Error err = FT_New_Face(ftData->lib, filePath.c_str(), 0, &ftData->face)) {
            CB_CORE_ERROR("FreeType: Failed to load font file \"{}\". Error Code = {}", filePath, err);
            return;
        }

        FT_Set_Pixel_Sizes(ftData->face, 0, fontSize);
        for(unsigned char c = 32; c < 128; c++) {
            loadGlyph(c);
        }
    }

    Glyph* Font::getGlyph(uint32_t c) {
        auto it = glyphs.find(c);
        if(it != glyphs.end()) {
            return &it->second;
        }
        return loadGlyph(c);
    }

    Glyph* Font::loadGlyph(uint32_t c) {
        if(FT_Error err = FT_Load_Char(ftData->face, c, FT_LOAD_RENDER)) {
            CB_CORE_ERROR("FreeType: Failed to load character '{}'. Error Code = {}", c, err);
            return nullptr;
        }
        Glyph glyph;
        glyph.size = {ftData->face->glyph->bitmap.width, ftData->face->glyph->bitmap.rows};
        glyph.bearing = {
            ftData->face->glyph->bitmap_left,
            ftData->face->glyph->bitmap_top,
        };
        glyph.advance = (ftData->face->glyph->advance.x * fontSize) / ftData->face->units_per_EM;

        // add glyph to atlas
        uint8_t* rawData = ftData->face->glyph->bitmap.buffer;
        int width = ftData->face->glyph->bitmap.width;
        int height = ftData->face->glyph->bitmap.rows;
        if(y + height > curSize) {
            curSize <<= 2;
            x = 0;
            y = 0;
            maxH = 0;
            if(curSize > MAX_TEX_SIZE) {
                curSize = MAX_TEX_SIZE;
                atlasPages.push_back(nullptr);
            }else{
                atlasPages.back().reset(nullptr);
                for(auto& g : glyphs) {
                    loadGlyph(g.first);
                }
            }
        }
        if(!atlasPages.back()) {
            uint8_t* d = new uint8_t[curSize * curSize * 4];
            atlasPages.back().reset(new Texture2D(curSize, curSize, d));
            delete[] d;
        }
        uint8_t* data = new uint8_t[curSize * curSize * 4];
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                data[i * width * 4 + j * 4] = 255;
                data[i * width * 4 + j * 4 + 1] = 255;
                data[i * width * 4 + j * 4 + 2] = 255;
                data[i * width * 4 + j * 4 + 3] = rawData[(height - i - 1) * width + j];
            }
        }
        atlasPages.back()->updateData(x, y, width, height, data);
        delete[] data;

        glyph.texture = atlasPages.back().get();
        glyph.texRegion.uvMin = glm::vec2(x, y + height) / (float)curSize;
        glyph.texRegion.uvMax = glm::vec2(x + width, y) / (float)curSize;
        glyphs[c] = glyph;

        x += width;
        maxH = std::max(y, maxH);
        if(x + width > curSize) {
            y += maxH;
            x = 0;
        }

        return &glyph;
    }

}  // namespace Cube