#pragma once

#include <string>

#define CB_COLOR(r, g, b) r / 255.0f, g / 255.0f, b / 255.0f
#define CB_COLOR4(r, g, b, a) r / 255.0f, g / 255.0f, b / 255.0f, a / 255.f

std::string getUserConfigDir();
