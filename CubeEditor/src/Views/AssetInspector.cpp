#include "AssetInspector.h"

#include "../Project/AssetExplorer.h"
#include "../App/EditorPage.h"
#include "../Project/Project.h"
#include "imgui/imgui.h"

void AssetInspector::render(float deltaTime) {
    ImGui::Begin("Asset Inspector");
    AssetNode* node = editorPage.selectedAssetNode;
    AssetExplorer& assetExplorer = editorPage.getProject()->getAssetExplorer();
    if (node && !node->isGroup) {
        ImGui::Text("identifier: %s", node->identifier.c_str());
        const nlohmann::json& importer = assetExplorer.getAssetImporter(node->identifier);
        switch(node->type){
            case Cube::ResourceType::Texture:{
                ImGui::Text("path:");
                ImGui::Text(importer["path"].get<std::string>().c_str());
                ImGui::Separator();
                // sprites
                // TODO: show sprite list
                if(ImGui::Button("load sprites from file")){
                    // TODO: load sprites from file
                }
                break;
            }
            case Cube::ResourceType::AnimationClip:{
                ImGui::Text("path:");
                ImGui::Text(importer["path"].get<std::string>().c_str());
                break;
            } 
            case Cube::ResourceType::Font:{
                ImGui::Text("fontFilePath:");
                ImGui::Text(importer["fontFilePath"].get<std::string>().c_str());
                ImGui::Text("fontSize:");
                ImGui::Text(std::to_string(importer["fontSize"].get<int>()).c_str());
                break;
            }
        }
        if(ImGui::Button("Reimport")) {
            // TODO: reimport resource
        }
        ImGui::Separator();
        // DEBUG
        ImGui::Text("importer:");
        ImGui::Text(importer.dump(4).c_str());
    } else {
        ImGui::Text("No Asset Selected");
    }

    ImGui::End();
}