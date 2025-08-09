#include "ImGuiExternal.h"

#include "Cube/Core/Log.h"

#include <cmath>

void addDashLine(ImDrawList* drawList, const ImVec2& start, const ImVec2& end, const ImU32& color, float thickness, float segmentLen, float intervalLen) {
    ImVec2 delta = end - start;
    float len = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    if(len < 1e-5f) {
        CB_EDITOR_ERROR("addDashLine: divided by zero");
        return;
    }
    ImVec2 unit = delta / len;
    float step = segmentLen + intervalLen;
    int count = (int)(len / step);
    for(int i = 0; i < count; ++i) {
        ImVec2 pos = start + unit * step * i;
        drawList->AddLine(pos, pos + unit * segmentLen, color, thickness);
    }
    if(step * count + segmentLen > len) {
        drawList->AddLine(start + unit * step * count, end, color, thickness);
    }else {
        drawList->AddLine( start + unit * step * count, start + unit * (step * count + segmentLen), color, thickness);
    }
}
