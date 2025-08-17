#include "ImGuiExternal.h"

#include "Cube/Core/Log.h"
#include "imgui/imgui_internal.h"

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
    } else {
        drawList->AddLine(start + unit * step * count, start + unit * (step * count + segmentLen), color, thickness);
    }
}

ModalPopup::ModalPopup(const std::string& title, const std::function<void()>& content, const std::function<void()>& confirm, const std::function<void()>& clear) : title(title), content(content), confirm(confirm), clear(clear) {
    originalBorderColor = ImGui::GetStyle().Colors[ImGuiCol_Border];
    borderColor = ImGui::GetStyle().Colors[ImGuiCol_Border];
}

void ModalPopup::render() {
    if(isOpen) {
        ImGui::OpenPopup(title.c_str());
    }else {
        clear();
    }
    ImGui::PushStyleColor(ImGuiCol_Border, borderColor);
    if(ImGui::BeginPopupModal(title.c_str(), &isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {

        content();

        constexpr float buttonWidth = 100.0f;
        constexpr float spacing = 100.0f;
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - (buttonWidth * 2 + ImGui::GetStyle().FramePadding.x * 2 + spacing)/ 2);
        ImGui::BeginGroup();
        if(ImGui::Button("OK", ImVec2(buttonWidth, 0)) || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
            confirm();
        }
        ImGui::SameLine(0.0f, spacing);
        if(ImGui::Button("Cancel", ImVec2(buttonWidth, 0))) {
            isOpen = false;
        }
        ImGui::EndGroup();

        if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            ImVec2 mousePos = ImGui::GetMousePos();
            ImVec2 winPos = ImGui::GetWindowPos();
            ImVec2 size = ImGui::GetWindowSize();
            if(!(mousePos.x >= winPos.x && mousePos.y >= winPos.y && mousePos.x <= winPos.x + size.x && mousePos.y <= winPos.y + size.y)) {
                borderColor = highLightColor;
                MessageBeep(MB_ICONASTERISK); //TODO: 跨平台适配
            }else {
                borderColor = originalBorderColor;
            }
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleColor();
}

bool IconTextButton(ImTextureID tex_id, const char* label, const ImVec2& icon_size, const ImVec2& uv_min, const ImVec2& uv_max, ImGuiButtonFlags flags) {
    float rounding = ImGui::GetStyle().FrameRounding;
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    // 计算文本尺寸
    ImVec2 text_size = ImGui::CalcTextSize(label);
    float padding = ImGui::GetStyle().FramePadding.y;

    // 计算整体大小（图标高度 + 文字高度 + 间距）
    ImVec2 total_size = ImVec2(
        ImMax(icon_size.x, text_size.x) + padding * 2,
        icon_size.y + text_size.y + padding * 3
    );

    // 创建透明按钮作为点击区域
    ImGui::InvisibleButton(label, total_size, flags);

    // 获取交互状态
    bool is_hovered = ImGui::IsItemHovered();
    bool is_active = ImGui::IsItemActive();
    bool is_clicked = ImGui::IsItemClicked();

    // 绘制按钮背景
    ImU32 bg_color = ImGui::GetColorU32(
        is_active ? ImGuiCol_ButtonActive : 
        is_hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button
    );

    ImGui::GetWindowDrawList()->AddRectFilled(
        ImGui::GetItemRectMin(), 
        ImGui::GetItemRectMax(), 
        bg_color, 
        rounding
    );

    // 添加边框效果
    if (is_hovered || is_active) {
        ImGui::GetWindowDrawList()->AddRect(
            ImGui::GetItemRectMin(), 
            ImGui::GetItemRectMax(), 
            ImGui::GetColorU32(ImGuiCol_Border), 
            rounding, 0, 1.5f
        );
    }

    // 计算图标位置（水平居中，顶部留边距）
    ImVec2 icon_pos = ImVec2(
        ImGui::GetItemRectMin().x + (total_size.x - icon_size.x) * 0.5f,
        ImGui::GetItemRectMin().y + padding
    );

    // 绘制图标（使用纹理）
    ImGui::GetWindowDrawList()->AddImage(
        tex_id,
        icon_pos,
        ImVec2(icon_pos.x + icon_size.x, icon_pos.y + icon_size.y),
        uv_min, uv_max
    );

    // 计算文本位置（水平居中，在图标下方）
    ImVec2 text_pos = ImVec2(
        ImGui::GetItemRectMin().x + (total_size.x - text_size.x) * 0.5f,
        icon_pos.y + icon_size.y + padding
    );

    // 绘制文本
    ImGui::GetWindowDrawList()->AddText(
        text_pos, 
        ImGui::GetColorU32(ImGuiCol_Text), 
        label
    );

    return is_clicked;
}