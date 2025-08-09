#pragma once

#include <imgui/imgui.h>

void addDashLine(ImDrawList* drawList, const ImVec2& start, const ImVec2& end, const ImU32& color, float thickness = 1.0f, float segmentLen = 10.0f, float intervalLen = 10.0f);