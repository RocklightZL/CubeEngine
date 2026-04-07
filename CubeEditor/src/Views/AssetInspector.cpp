#include "AssetInspector.h"

#include <string>

#include "Cube/UI/FileDialog.h"
#include "../Project/AssetExplorer.h"
#include "../App/EditorApp.h"
#include "../App/EditorPage.h"
#include "../Project/Project.h"
#include "../Utils/misc.h"
#include "imgui/imgui.h"

void AssetInspector::render(float deltaTime) {
    ImGui::Begin("Asset Inspector");
    AssetNode* node = editorPage.selectedAssetNode;
    AssetExplorer& assetExplorer = editorPage.getProject()->getAssetExplorer();
    if (node && !node->isGroup) {
        static std::string editingIdentifier;
        static nlohmann::json editingImporter;

        ImGui::Text("identifier: %s", node->identifier.c_str());
        const nlohmann::json& importer = assetExplorer.getAssetImporter(node->identifier);
        if(editingIdentifier != node->identifier) {
            editingIdentifier = node->identifier;
            editingImporter = importer;
        }

        switch(node->type){
            case Cube::ResourceType::Texture:{
                ImGui::Text("path:");
                ImGui::Text("%s", editingImporter.value("path", "").c_str());
                ImGui::Separator();

                if(ImGui::Button("Load Sprites From Atlas")) {
                    std::string atlasPath = Cube::FileDialog::openFile("Atlas JSON(.json)\0*.json\0All(.*)\0*.*\0", EditorApp::get().getWindow()->getWin32Window());
                    if(!atlasPath.empty()) {
                        editingImporter["sprites"] = Utils::parseAtlasFile(atlasPath);
                    }
                }
                ImGui::SameLine();
                if(ImGui::Button("Clear Sprites")) {
                    editingImporter.erase("sprites");
                }

                if(editingImporter.contains("sprites") && editingImporter["sprites"].is_object()) {
                    ImGui::Text("sprites: %d", static_cast<int>(editingImporter["sprites"].size()));
                    if(ImGui::TreeNode("Sprite UV List")) {
                        for(auto& sprite : editingImporter["sprites"].items()) {
                            if(!sprite.value().is_array() || sprite.value().size() < 4) {
                                continue;
                            }
                            float uv[4] = {
                                sprite.value()[0].get<float>(),
                                sprite.value()[1].get<float>(),
                                sprite.value()[2].get<float>(),
                                sprite.value()[3].get<float>()
                            };
                            ImGui::PushID(sprite.key().c_str());
                            ImGui::InputFloat4(sprite.key().c_str(), uv, "%.4f");
                            sprite.value() = {uv[0], uv[1], uv[2], uv[3]};
                            ImGui::PopID();
                        }
                        ImGui::TreePop();
                    }
                }
                break;
            }
            case Cube::ResourceType::AnimationClip:{
                ImGui::Text("path:");
                ImGui::Text("%s", editingImporter.value("path", "").c_str());
                break;
            } 
            case Cube::ResourceType::Font:{
                ImGui::Text("fontFilePath:");
                ImGui::Text("%s", editingImporter.value("fontFilePath", "").c_str());
                ImGui::Text("fontSize:");
                int fontSize = editingImporter.value("fontSize", 16);
                if(ImGui::InputInt("##fontSize", &fontSize)) {
                    if(fontSize < 1) {
                        fontSize = 1;
                    }
                    editingImporter["fontSize"] = fontSize;
                }
                break;
            }
            default: break;
        }
        if(ImGui::Button("Reimport")) {
            assetExplorer.reimportResource(node->identifier, editingImporter);
            editingImporter = assetExplorer.getAssetImporter(node->identifier);
        }
        ImGui::Separator();
        // DEBUG
        ImGui::Text("importer:");
        ImGui::Text("%s", editingImporter.dump(4).c_str());
    } else {
        ImGui::Text("No Asset Selected");
    }

    ImGui::End();
}