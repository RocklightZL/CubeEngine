#include "AnimationEditor.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "../App/EditorPage.h"
#include "../Project/AssetExplorer.h"
#include "../Project/Project.h"
#include "../Utils/EditorTextureCache.h"
#include "Cube/Core/Log.h"
#include "Cube/Renderer/TextureRegion.h"
#include "json.hpp"

#include <imgui/imgui.h>

namespace {

bool resolveSpritePreview(const std::string& spriteIdentifier,
                        AssetExplorer& assetExplorer,
                        Cube::Texture2D*& texture,
                        Cube::TextureRegion& texRegion) {
    texture = nullptr;
    texRegion = {{0.0f, 0.0f}, {1.0f, 1.0f}};

    if(spriteIdentifier.rfind("spr:", 0) != 0) {
        return false;
    }

    const std::string resourcePart = spriteIdentifier.substr(4);
    const size_t hashPos = resourcePart.find('#');
    const std::string texIdentifier = hashPos == std::string::npos ? resourcePart : resourcePart.substr(0, hashPos);

    nlohmann::json importer;
    try {
        importer = assetExplorer.getAssetImporter(texIdentifier);
    } catch(const std::out_of_range&) {
        return false;
    }

    const std::string texPath = importer.value("path", "");
    if(texPath.empty()) {
        return false;
    }

    texture = EditorTextureCache::get().request(texPath);
    if(!texture) {
        return false;
    }

    if(hashPos != std::string::npos) {
        const std::string spriteName = resourcePart.substr(hashPos + 1);
        if(importer.contains("sprites") && importer["sprites"].is_object() && importer["sprites"].contains(spriteName)) {
            const auto& uv = importer["sprites"][spriteName];
            if(uv.is_array() && uv.size() >= 4) {
                texRegion = {
                    {uv[0].get<float>(), uv[1].get<float>()},
                    {uv[2].get<float>(), uv[3].get<float>()}
                };
            }
        }
    }

    return true;
}

} // namespace

bool AnimationEditor::createNewAnimationClip(const std::string& fileName) {
    Project* project = editorPage.getProject();
    if(!project) {
        // TODO: Show user-level error prompt in unified notification system.
        CB_EDITOR_ERROR("AnimationEditor: No active project when creating animation clip");
        return false;
    }

    namespace fs = std::filesystem;
    const fs::path assetsDir(project->getConfig().assetsDirectory);

    std::string baseName = fileName;
    if(baseName.empty()) {
        baseName = "NewAnimation";
    }
    if(baseName.size() > 5 && baseName.substr(baseName.size() - 5) == ".anim") {
        baseName = baseName.substr(0, baseName.size() - 5);
    }

    fs::path animPath = assetsDir / (baseName + ".anim");
    int index = 1;
    while(fs::exists(animPath)) {
        animPath = assetsDir / (baseName + "_" + std::to_string(index) + ".anim");
        ++index;
    }

    nlohmann::json animData;
    animData["name"] = animPath.stem().string();
    animData["looping"] = true;
    animData["speed"] = 1.0f;
    animData["duration"] = 0.0f;
    animData["frames"] = nlohmann::json::array();

    std::ofstream file(animPath);
    if(!file.is_open()) {
        // TODO: Show user-level error prompt in unified notification system.
        CB_EDITOR_ERROR("AnimationEditor: Failed to create animation file {}", animPath.string());
        return false;
    }
    file << animData.dump(4);
    file.close();

    const fs::path absPath = fs::weakly_canonical(animPath);
    const fs::path relPath = fs::relative(absPath, assetsDir);

    nlohmann::json importConfig;
    importConfig["path"] = absPath.generic_string();
    project->getAssetExplorer().createResource("anim:" + relPath.generic_string(), importConfig);

    TargetChangeEvent e(absPath.generic_string());
    EditorApp::get().getEventDispatcher().dispatch(e);
    return true;
}

bool AnimationEditor::loadTargetAnim() {
    frames.clear();
    name.clear();
    looping = false;
    speed = 1.0f;
    duration = 0.0f;

    if(target.empty()) {
        return false;
    }

    std::ifstream file(target);
    if(!file.is_open()) {
        // TODO: Show user-level error prompt in unified notification system.
        CB_EDITOR_ERROR("AnimationEditor: Failed to open animation file {}", target);
        return false;
    }

    nlohmann::json animData;
    try {
        file >> animData;
    } catch(const nlohmann::json::exception& e) {
        // TODO: Show user-level error prompt in unified notification system.
        CB_EDITOR_ERROR("AnimationEditor: Invalid json file {}: {}", target, e.what());
        return false;
    }

    name = animData.value("name", "");
    looping = animData.value("looping", false);
    speed = animData.value("speed", 1.0f);
    duration = 0.0f;

    if(animData.contains("frames") && animData["frames"].is_array()) {
        for(const auto& frameJson : animData["frames"]) {
            FrameViewData f;
            f.frame = frameJson.value("frame", "");
            f.duration = frameJson.value("duration", 0.0f);
            if(f.duration < 0.0f) {
                f.duration = 0.0f;
            }
            duration += f.duration;
            frames.push_back(std::move(f));
        }
    }

    return true;
}

void AnimationEditor::render(float deltaTime) {
    ImGui::Begin("Animation Editor");

    static char newAnimName[128] = "NewAnimation";

    if(ImGui::Button("New AnimationClip")) {
        ImGui::OpenPopup("New AnimationClip");
    }

    if(ImGui::BeginPopupModal("New AnimationClip", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Input animation file name:");
        ImGui::InputText("##NewAnimName", newAnimName, IM_ARRAYSIZE(newAnimName));

        if(ImGui::Button("Create", ImVec2(120.0f, 0.0f))) {
            createNewAnimationClip(newAnimName);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel", ImVec2(120.0f, 0.0f))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::Text("Target:");
    ImGui::SameLine();
    ImGui::Text("%s", target.empty() ? "<None>" : target.c_str());

    if(target.empty()) {
        ImGui::End();
        return;
    }

    ImGui::Separator();
    ImGui::Text("name: %s", name.c_str());
    ImGui::Checkbox("looping", &looping);
    if(ImGui::InputFloat("speed", &speed, 0.1f, 1.0f, "%.3f")) {
        if(speed < 0.0f) {
            speed = 0.0f;
        }
    }

    float autoDuration = 0.0f;
    for(const auto& f : frames) {
        autoDuration += f.duration;
    }
    duration = autoDuration;
    ImGui::Text("duration(auto): %.3f", duration);
    ImGui::Text("frames: %d", static_cast<int>(frames.size()));

    ImGui::Separator();
    if(ImGui::Button("Add Frame")) {
        // TODO: frame insert workflow will be added next.
    }
    ImGui::SameLine();
    ImGui::TextDisabled("(TODO)");

    ImGui::Separator();
    ImGui::BeginChild("AnimationFrames", ImVec2(0, 360), true);
    AssetExplorer& assetExplorer = editorPage.getProject()->getAssetExplorer();

    for(size_t i = 0; i < frames.size(); ++i) {
        const auto& frame = frames[i];
        ImGui::PushID(static_cast<int>(i));

        ImGui::Text("Frame %d", static_cast<int>(i));
        ImGui::Text("identifier: %s", frame.frame.c_str());
        ImGui::Text("frame duration: %.3f", frame.duration);

        Cube::Texture2D* previewTexture = nullptr;
        Cube::TextureRegion region = {{0.0f, 0.0f}, {1.0f, 1.0f}};
        if(resolveSpritePreview(frame.frame, assetExplorer, previewTexture, region) && previewTexture) {
            const float regionW = static_cast<float>(previewTexture->getWidth()) * (region.uvMax.x - region.uvMin.x);
            const float regionH = static_cast<float>(previewTexture->getHeight()) * (region.uvMax.y - region.uvMin.y);
            const float maxSize = 72.0f;
            float scale = 1.0f;
            if(regionW > 0.0f && regionH > 0.0f) {
                scale = std::min(maxSize / regionW, maxSize / regionH);
            }
            const ImVec2 drawSize = {regionW * scale, regionH * scale};
            ImGui::Image(previewTexture->getId(), drawSize, {region.uvMin.x, region.uvMax.y}, {region.uvMax.x, region.uvMin.y});
        } else {
            ImGui::TextDisabled("Preview unavailable");
        }

        ImGui::Separator();
        ImGui::PopID();
    }

    ImGui::EndChild();

    ImGui::End();
}

bool AnimationEditor::onTargetChange(const Cube::Event& e) {
    const TargetChangeEvent& event = static_cast<const TargetChangeEvent&>(e);
    target = event.targetFilePath;
    loadTargetAnim();
    ImGui::SetWindowFocus("Animation Editor");
    return true;
}