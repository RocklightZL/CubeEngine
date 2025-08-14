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
                MessageBeep(MB_ICONASTERISK); //TODO: ¿çÆ½Ì¨ÊÊÅä
            }else {
                borderColor = originalBorderColor;
            }
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleColor();
}
