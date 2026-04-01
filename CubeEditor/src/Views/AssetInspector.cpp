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
        ImGui::Text("importer:");
        ImGui::Text(assetExplorer.getAssetImporter(node->identifier).dump(4).c_str());
    } else {
        ImGui::Text("No Asset Selected");
    }

    ImGui::End();
}