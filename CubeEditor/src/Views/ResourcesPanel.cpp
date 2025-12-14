#include "ResourcesPanel.h"

#include <memory>
#include <stack>
#include <filesystem>

#include "../App/EditorApp.h"
#include "../Project.h"
#include "../Scene/TextureMetadata.h"
#include "../Utils/ImGuiExternal.h"
#include "Cube/Core/Log.h"
#include "Cube/Renderer/Renderer.h"
#include "Cube/Resource/ResourceManager.h"
#include "Cube/UI/FileDialog.h"
#include "Cube/Utils/Utils.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

using namespace Cube;

namespace fs = std::filesystem;

extern Project* proj;
extern EditorApp* app;

void ResourcesPanel::render(float deltaTime) {
    static int showMode = 0; // 0: icon mode 1: list mode
    ImGui::Begin("Resources Panel");
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

    static const fs::path ASSETS_DIR(proj->getConfig().assetsDirectory);
    static fs::path path("");
    const std::string topText = "Assets: " + path.string();
    float topHeight = ImGui::CalcTextSize(topText.c_str()).y;

    ImGui::BeginChild("TopBar", ImVec2(ImGui::GetContentRegionAvail().x, topHeight));
    topHeight -= ImGui::GetStyle().FramePadding.x * 2;
    if(ImGui::ImageButton("back", back_png->getId(), ImVec2(topHeight, topHeight), {0, 1}, {1, 0})) {
        path = path.parent_path();
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
    static fs::path selectedEntry;
    for(const auto& entry : fs::directory_iterator(ASSETS_DIR / path)) {
        ImGui::PushID(&entry);
        if(showMode == 0) {
            ImGui::SameLine();
            if(ImGui::GetContentRegionAvail().x < imageSize) {
                ImGui::NewLine();
            }
            if(entry.is_directory()){
                iconTextButton(directory_png.get(), entry.path().filename().string(), selectedEntry == entry.path(), ImVec2(imageSize, imageSize));
                if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    path = path / entry.path().filename();
                }
            }else if(entry.is_regular_file() && entry.path().extension() == ".meta"){
                RUID ruid = proj->assetMetaCache[path];
                AssetMeta* assetMeta = ResourceManager::get().getAssetMeta(ruid);

                iconTextButton(file_png.get(), entry.path().filename().string(), selectedEntry == entry.path(), ImVec2(imageSize, imageSize));
                if(ImGui::BeginDragDropSource()) {
                    ImGui::Text(std::to_string(ruid).c_str());
                    ImGui::SetDragDropPayload("AssetRUID", &ruid, sizeof(ruid));
                    ImGui::EndDragDropSource();
                }
            }
            if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                selectedEntry = entry.path();
            }
            if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                ImGui::OpenPopup("NodeRightButtonMenu");
            }
        }else if(showMode == 1){
            if(entry.is_directory()){
                iconTextButtonH(directory_png.get(), entry.path().filename().string(), selectedEntry == entry.path());
                if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    path = path / entry.path().filename();
                }
            }else {
                iconTextButtonH(file_png.get(), entry.path().filename().string(), selectedEntry == entry.path());
                if(ImGui::BeginDragDropSource()) {
                    RUID ruid = proj->assetMetaCache[path];
                    ImGui::Text(std::to_string(ruid).c_str());
                    ImGui::SetDragDropPayload("AssetRUID", &ruid, sizeof(ruid));
                    ImGui::EndDragDropSource();
                }
            }
            if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                selectedEntry = entry.path();
            }
            if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                ImGui::OpenPopup("NodeRightButtonMenu");
            }
        }
        ImGui::PopID();
    }
    if(ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered()) {
        selectedEntry = "";
    }
    ImGui::EndChild();

    ImGui::PopStyleColor();
    ImGui::End();

    ImGui::Begin("ResourcesPreview");

    ImGui::End();
}

void ResourcesPanel::importResources() {
    std::shared_ptr<Node> currentNode = proj->resStack.back();
    for(auto& path : FileDialog::openMultiFiles("Resources(.png.jpg)\0*.png;*.jpg\0", app->getWindow()->getWin32Window())) {
        std::string fileName = Utils::getFileName(path, true);
        if(!Utils::isFileInDirectory(path, proj->getConfig().assetsDirectory)) {
            Utils::copyFile(path, proj->getConfig().assetsDirectory + "/" + fileName);
        }
        if(std::find_if(currentNode->children.begin(), currentNode->children.end(), [fileName](std::shared_ptr<Node> x){ return x->name == fileName; }) == currentNode->children.end()){
            std::shared_ptr<Node> n = std::make_shared<Node>();
            n->isGroup = false;
            n->name = fileName;
            currentNode->children.push_back(n);
        } // TODO: 提醒用户不能添加重复资源
    }
}