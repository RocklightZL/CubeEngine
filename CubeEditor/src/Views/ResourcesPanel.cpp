#include "ResourcesPanel.h"

#include <filesystem>
#include <fstream>
#include <memory>
#include <stack>

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
    fs::path& path = proj->currentAssetsDir;
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
    if(showMode == 0){
        for(const auto& entry : fs::directory_iterator(ASSETS_DIR / path)) {
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
                RUID ruid = proj->assetMetaCache[entry.path()];
                iconTextButton(file_png.get(), entry.path().filename().string(), selectedEntry == entry.path(), ImVec2(imageSize, imageSize));
                if(ImGui::BeginDragDropSource()) {
                    ImGui::Text(std::to_string(ruid).c_str());
                    ImGui::SetDragDropPayload("AssetRUID", &ruid, sizeof(ruid));
                    ImGui::EndDragDropSource();
                }
            }else {
                goto end;
            }
            if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                selectedEntry = entry.path();
            }
            if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                ImGui::OpenPopup("NodeRightButtonMenu");
            }
            end:;
        }
    } else if(showMode == 1){
        for(const auto& entry : fs::directory_iterator(ASSETS_DIR / path)) {
            if(entry.is_directory()){
                iconTextButtonH(directory_png.get(), entry.path().filename().string(), selectedEntry == entry.path());
                if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    path = path / entry.path().filename();
                }
            }else {
                iconTextButtonH(file_png.get(), entry.path().filename().string(), selectedEntry == entry.path());
                // if(ImGui::BeginDragDropSource()) {
                //     RUID ruid = proj->assetMetaCache[path];
                //     ImGui::Text(std::to_string(ruid).c_str());
                //     ImGui::SetDragDropPayload("AssetRUID", &ruid, sizeof(ruid));
                //     ImGui::EndDragDropSource();
                // }
            }
            if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                selectedEntry = entry.path();
            }
            if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                ImGui::OpenPopup("NodeRightButtonMenu");
            }
        }
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

void ResourcesPanel::importFromFileDialog() {
    for(auto& path : FileDialog::openMultiFiles("Resources(.png.jpg)\0*.png;*.jpg\0All(.*)\0*.*\0", app->getWindow()->getWin32Window())) {
        importResource(path);
    }
    proj->updateAssetMetaCache();
}

void importRes(const std::filesystem::path& source, const std::filesystem::path& target) {
    if(std::filesystem::is_directory(source)) {
        if(!std::filesystem::exists(target)) {
            std::filesystem::create_directories(target);
        }
        for(auto& entry : std::filesystem::directory_iterator(source)) {
            importRes(entry.path(), target / entry.path().filename());
        }
    }else {
        if(std::filesystem::exists(target.string() + ".meta")) {
            CB_EDITOR_WARN("Asset {} is already imported", target.string());
            return;
        }
        if(target != source) {
            std::error_code ec;
            std::filesystem::copy_file(source, target, std::filesystem::copy_options::none, ec);
            if(ec) {
                CB_EDITOR_ERROR("Failed to copy file from {} to {}. Error Code: {}", source.string(), target.string(), ec.message());
                return;
            }
        }
        AssetMeta assetMeta;
        if(target.extension() == ".png" || target.extension() == ".jpg") {
            assetMeta.ruid = RUIDGenerator::gen(ResourceType::Texture);
        } else if(target.extension() == ".anim") {
            assetMeta.ruid = RUIDGenerator::gen(ResourceType::AnimationClip);
        }
        else {
            CB_EDITOR_ERROR("Unknown assets format: {}", source.extension().string());
            return;
        }
        assetMeta.sourcePath = std::filesystem::canonical(target).string();
        assetMeta.writeToFile(target.string() + ".meta");
    }
}

void ResourcesPanel::importResource(const std::string& path) {
    std::filesystem::path filepath(path);
    std::filesystem::path targetFile = proj->getConfig().assetsDirectory / proj->currentAssetsDir / filepath.filename();
    importRes(path, targetFile);
}