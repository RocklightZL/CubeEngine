#pragma once

namespace Cube {

    struct Color {
        float r;
        float g;
        float b;
        float a;
        Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
        Color(int r, int g, int b, int a) : r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(a / 255.0f) {}
        Color(uint32_t hex) : r((hex >> 24 & 0xFF) / 255.0f), g((hex >> 16 & 0xFF) / 255.0f), b((hex >> 8 & 0xFF) / 255.0f), a((hex & 0xFF) / 255.0f) {}

        constexpr static Color BLACK    = {0.0f, 0.0f, 0.0f, 1.0f};
        constexpr static Color WHITE    = {1.0f, 1.0f, 1.0f, 1.0f};
        constexpr static Color RED      = {1.0f, 0.0f, 0.0f, 1.0f};
        constexpr static Color GREEN    = {0.0f, 1.0f, 0.0f, 1.0f};
        constexpr static Color BLUE     = {0.0f, 0.0f, 1.0f, 1.0f};
        constexpr static Color CYAN     = {0.0f, 1.0f, 1.0f, 1.0f};
        constexpr static Color MAGENTA  = {1.0f, 0.0f, 1.0f, 1.0f};
        constexpr static Color YELLOW   = {1.0f, 1.0f, 0.0f, 1.0f};
    };
}