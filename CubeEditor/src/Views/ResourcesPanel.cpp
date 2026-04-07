#include "ResourcesPanel.h"

#include <filesystem>
#include <fstream>
#include <memory>
#include <stack>

#include "../App/EditorPage.h"
#include "../App/EditorApp.h"
#include "../Project/Project.h"
#include "../Utils/ImGuiExternal.h"
#include "Cube/Core/Log.h"
#include "Cube/Renderer/Renderer.h"
#include "Cube/UI/FileDialog.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

using namespace Cube;

namespace fs = std::filesystem;

struct SelectedManager {
    std::unordered_set<AssetNode*> nodes;

    void singleSelect(AssetNode* node) {
        nodes.clear();
        nodes.insert(node);
    }

    void multiSelect(AssetNode* node) {
        nodes.insert(node);
    }

    bool isSelected(AssetNode* node) {
        return nodes.count(node);
    }

    void cancel() {
        nodes.clear();
    }

    AssetNode* getSingleNode() const {
        return nodes.empty() ? nullptr : *(nodes.begin());
    }

    const std::unordered_set<AssetNode*>& getNodes() const {
        return nodes;
    }
};

void ResourcesPanel::render(float deltaTime) {
    Project* project = editorPage.getProject();
    AssetExplorer& assetExplorer = project->getAssetExplorer();
    static int showMode = 0; // 0: icon mode 1: list mode
    ImGui::Begin("Resources Panel");
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

    static const fs::path ASSETS_DIR(project->getConfig().assetsDirectory);
    const std::string topText = assetExplorer.getCurrentPath();
    float topHeight = ImGui::CalcTextSize(topText.c_str()).y;

    ImGui::BeginChild("TopBar", ImVec2(ImGui::GetContentRegionAvail().x, topHeight));
    topHeight -= ImGui::GetStyle().FramePadding.x * 2;
    if(ImGui::ImageButton("back", back_png->getId(), ImVec2(topHeight, topHeight), {0, 1}, {1, 0})) {
        assetExplorer.back();
    }
    ImGui::SameLine();
    ImGui::Text(topText.c_str());
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - topHeight - ImGui::GetStyle().FramePadding.x * 2);
    if(showMode == 0) {
        if(ImGui::ImageButton("iconMode", icon_mode_png->getId(), ImVec2(topHeight, topHeight))) {
            showMode = 1;
        }
    }else if(showMode == 1) {
        if(ImGui::ImageButton("listMode", list_mode_png->getId(), ImVec2(topHeight, topHeight))) {
            showMode = 0;
        }
    }
    ImGui::EndChild();

    ImGui::BeginChild("Content", ImGui::GetContentRegionAvail());
    constexpr float imageSize = 128.0f;
    static SelectedManager selectedManager;
    struct {
        AssetNode* src = nullptr;
        AssetNode* dst = nullptr;
    } move;
    if(showMode == 0){
        for(const auto& entry : assetExplorer.getCurrentNode()->children) {
            ImGui::SameLine();
            if(ImGui::GetContentRegionAvail().x < imageSize) {
                ImGui::NewLine();
            }
            if(entry->isGroup){
                iconTextButton(directory_png.get(), entry->name, selectedManager.isSelected(entry.get()), ImVec2(imageSize, imageSize));
                if(ImGui::BeginDragDropTarget()) {
                    if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset")) {
                        move.src = *(AssetNode**)payload->Data;
                        move.dst = entry.get();
                    }
                    ImGui::EndDragDropTarget();
                }
            }else {
                switch(entry->type) {
                    case ResourceType::Texture:
                        {
                            Texture2D* tex = thumbnailManager.request(assetExplorer.getAssetImporter(entry->identifier)["path"].get<std::string>());
                            if(!tex) tex = file_png.get();
                            iconTextButton(tex, entry->name, selectedManager.isSelected(entry.get()), ImVec2(imageSize, imageSize));
                        }
                        break;
                    case ResourceType::AnimationClip:
                        iconTextButton(file_png.get(), entry->name, selectedManager.isSelected(entry.get()), ImVec2(imageSize, imageSize));
                        break;
                    default:
                        iconTextButton(file_png.get(), entry->name, selectedManager.isSelected(entry.get()), ImVec2(imageSize, imageSize));
                        break;
                }
                ImGui::PushStyleColor(ImGuiCol_PopupBg, {0, 0, 0, 0});
                ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0);
                if(ImGui::BeginDragDropSource()) {
                    AssetNode* src = entry.get();
                    ImGui::SetDragDropPayload("Asset", &src, sizeof(src));
                    Texture2D* tex = thumbnailManager.request(assetExplorer.getAssetImporter(src->identifier)["path"].get<std::string>());
                    ImGui::Image(tex ? tex->getId() : file_png->getId(), {64, 64}, {0, 1}, {1, 0});
                    ImGui::EndDragDropSource();
                }
                ImGui::PopStyleVar();
                ImGui::PopStyleColor();
            }
            if(entry->isGroup && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                assetExplorer.enterNode(entry.get());
                selectedManager.cancel();
            }else{
                if(ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
                    if(ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
                        selectedManager.multiSelect(entry.get());
                    }else {
                        selectedManager.singleSelect(entry.get());
                    }
                }
            }
            if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                selectedManager.singleSelect(entry.get());
                ImGui::OpenPopup("NodeRightMenu");
            }
        }
    } else if(showMode == 1){
        // for(const auto& entry : proj->assetExplorer.getCurrentNode()->children) {
        //     if(entry->isGroup){
        //         iconTextButtonH(directory_png.get(), entry->name, selectedEntry == entry.get());
        //         if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
        //             proj->assetExplorer.enterNode(entry.get());
        //         }
        //     }else {
        //         iconTextButtonH(file_png.get(), entry->name, selectedEntry == entry.get());
        //     }
        //     if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        //         selectedEntry = entry.get();
        //     }
        //     if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        //         ImGui::OpenPopup("NodeRightButtonMenu");
        //     }
        // }
    }
    if(ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()) {
        if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            selectedManager.cancel();
        }
        if(ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            selectedManager.cancel();
            ImGui::OpenPopup("BlankRightMenu");
        }
    }

    static std::vector<AssetNode*> cutNodes;
    if(ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
        // Ctrl+X - cut
        if(ImGui::IsKeyPressed(ImGuiKey_X)) {
            if(!cutNodes.empty()) cutNodes.clear();
            for(auto& n : selectedManager.getNodes()) {
                cutNodes.push_back(n);
            }
        }
        // Ctrl+V - paste
        if(ImGui::IsKeyPressed(ImGuiKey_V)) {
            for(auto& n : cutNodes) {
                assetExplorer.move(n, assetExplorer.getCurrentNode());
            }
            cutNodes.clear();
        }
    }

    // delay move
    if(move.src && move.dst) {
        assetExplorer.move(move.src, move.dst);
    }

    static char inputBuf[50] = {};
    static std::unique_ptr<ModalPopup> renamePopup = std::make_unique<ModalPopup>("Rename", [] {
        ImGui::Text("Name:");
        ImGui::SetKeyboardFocusHere();
        ImGui::InputText("##rename", inputBuf, IM_ARRAYSIZE(inputBuf), ImGuiInputTextFlags_AutoSelectAll);
    }, [] {
        selectedManager.getSingleNode()->name = inputBuf;
        renamePopup->close();
    }, [] {});

    if(ImGui::BeginPopup("NodeRightMenu")) {
        if(selectedManager.getSingleNode()->isGroup) {
            if(ImGui::MenuItem("Rename")) {
                strcpy_s(inputBuf, selectedManager.getSingleNode()->name.c_str());
                renamePopup->open();
            }
        }
        if(ImGui::MenuItem("Delete")) {
            assetExplorer.removeNode(selectedManager.getSingleNode());
            selectedManager.cancel();
        }
        ImGui::EndPopup();
    }
    if(ImGui::BeginPopup("BlankRightMenu")) {
        if(ImGui::MenuItem("New Group")) {
            selectedManager.singleSelect(assetExplorer.createGroup("Group"));
            strcpy_s(inputBuf, selectedManager.getSingleNode()->name.c_str());
            renamePopup->open();
        }
        ImGui::EndPopup();
    }

    editorPage.selectedAssetNode = selectedManager.getSingleNode();

    ImGui::EndChild();

    ImGui::PopStyleColor();

    renamePopup->render();

    ImGui::End();

    // ImGui::Begin("ResourcesPreview");
    //
    // ImGui::End();

    thumbnailManager.tick();
}

