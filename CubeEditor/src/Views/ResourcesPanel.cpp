#include "ResourcesPanel.h"

#include <filesystem>
#include <fstream>
#include <memory>
#include <stack>

#include "../App/EditorApp.h"
#include "../Project.h"
#include "../Utils/ImGuiExternal.h"
#include "Cube/Core/Log.h"
#include "Cube/Renderer/Renderer.h"
#include "Cube/UI/FileDialog.h"
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
    const std::string topText = proj->assetExplorer.getCurrentPath();
    float topHeight = ImGui::CalcTextSize(topText.c_str()).y;

    ImGui::BeginChild("TopBar", ImVec2(ImGui::GetContentRegionAvail().x, topHeight));
    topHeight -= ImGui::GetStyle().FramePadding.x * 2;
    if(ImGui::ImageButton("back", back_png->getId(), ImVec2(topHeight, topHeight), {0, 1}, {1, 0})) {
        proj->assetExplorer.back();
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
    static AssetNode* selectedEntry;
    if(showMode == 0){
        for(const auto& entry : proj->assetExplorer.getCurrentNode()->children) {
            ImGui::SameLine();
            if(ImGui::GetContentRegionAvail().x < imageSize) {
                ImGui::NewLine();
            }
            if(entry->isGroup){
                iconTextButton(directory_png.get(), entry->name, selectedEntry == entry.get(), ImVec2(imageSize, imageSize));
                if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    proj->assetExplorer.enterNode(entry.get());
                }
            }else {
                switch(entry->type) {
                    case ResourceType::Texture:
                        {
                            Texture2D* tex = thumbnailManager.request(proj->assetPathMap.value(entry->identifier, ""));
                            if(!tex) tex = file_png.get();
                            iconTextButton(tex, entry->name, selectedEntry == entry.get(), ImVec2(imageSize, imageSize));
                        }
                        break;
                    case ResourceType::AnimationClip:
                        iconTextButton(file_png.get(), entry->name, selectedEntry == entry.get(), ImVec2(imageSize, imageSize));
                        break;
                    case ResourceType::Atlas:
                        iconTextButton(file_png.get(), entry->name, selectedEntry == entry.get(), ImVec2(imageSize, imageSize));
                        break;
                    default:
                        iconTextButton(file_png.get(), entry->name, selectedEntry == entry.get(), ImVec2(imageSize, imageSize));
                        break;
                }
                // iconTextButton(file_png.get(), entry->name, selectedEntry == entry.get(), ImVec2(imageSize, imageSize));
            }
            if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                selectedEntry = entry.get();
            }
            if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                ImGui::OpenPopup("NodeRightButtonMenu");
            }
        }
    } else if(showMode == 1){
        for(const auto& entry : proj->assetExplorer.getCurrentNode()->children) {
            if(entry->isGroup){
                iconTextButtonH(directory_png.get(), entry->name, selectedEntry == entry.get());
                if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    proj->assetExplorer.enterNode(entry.get());
                }
            }else {
                iconTextButtonH(file_png.get(), entry->name, selectedEntry == entry.get());
            }
            if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                selectedEntry = entry.get();
            }
            if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                ImGui::OpenPopup("NodeRightButtonMenu");
            }
        }
    }
    if(ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered()) {
        selectedEntry = nullptr;
    }
    ImGui::EndChild();

    ImGui::PopStyleColor();
    ImGui::End();

    ImGui::Begin("ResourcesPreview");

    ImGui::End();

    thumbnailManager.tick();
}

void ResourcesPanel::importFromFileDialog() {
    for(auto& path : FileDialog::openMultiFiles("Resources(.png.jpg)\0*.png;*.jpg\0All(.*)\0*.*\0", app->getWindow()->getWin32Window())) {
        importResource(path);
    }
}

void importTexture(const std::filesystem::path& texturePath) {
    std::filesystem::path path = std::filesystem::canonical(texturePath);
    std::filesystem::path relPath = std::filesystem::relative(path, proj->getConfig().assetsDirectory);
    proj->assetPathMap["tex:" + relPath.generic_string()] = path.generic_string();
    proj->assetExplorer.addNode(new AssetNode{path.filename().string(), "tex:" + relPath.generic_string(), ResourceType::Texture});
}

void importAnimClip(const std::filesystem::path& animPath) {
    std::filesystem::path path = std::filesystem::canonical(animPath);
    std::filesystem::path relPath = std::filesystem::relative(path, proj->getConfig().assetsDirectory);
    proj->assetPathMap["anim:" + relPath.generic_string()] = path.generic_string();
    proj->assetExplorer.addNode(new AssetNode{path.filename().string(),"anim:" + relPath.generic_string(), ResourceType::AnimationClip});
}

void importAtlas(const std::filesystem::path& atlasPath) {
    std::filesystem::path path = std::filesystem::canonical(atlasPath);
    std::filesystem::path relPath = std::filesystem::relative(path, proj->getConfig().assetsDirectory);
    proj->assetPathMap["atlas:" + relPath.generic_string()] = path.generic_string();
    proj->assetExplorer.addNode(new AssetNode{path.filename().string(),"atlas:" + relPath.generic_string(), ResourceType::Atlas});
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
        if(target != source) {
            std::error_code ec;
            std::filesystem::copy_file(source, target, std::filesystem::copy_options::none, ec);
            if(ec) {
                CB_EDITOR_ERROR("Failed to copy file from {} to {}. Error Code: {}", source.string(), target.string(), ec.message());
                return;
            }
        }
        if(target.extension() == ".png" || target.extension() == ".jpg") {
            importTexture(target);
        } else if(target.extension() == ".anim") {
            importAnimClip(target);
        } else if(target.extension() == ".atlas") {
            importAtlas(target);
        }
        else {
            CB_EDITOR_ERROR("Unknown assets format: {}", source.extension().string());
            return;
        }
    }
}

void ResourcesPanel::importResource(const std::string& path) {
    std::filesystem::path filepath(path);
    std::filesystem::path targetFile = proj->getConfig().assetsDirectory;
    targetFile /= filepath.filename();
    importRes(path, targetFile);
    proj->refreshResourceManager();
}