#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "../Project/AssetExplorer.h"

class EditorPage;

class ResourcePickerDialog {
public:
    void open(const std::string& popupTitle,
              AssetNode* rootNode,
              Cube::ResourceType allowedType);

    bool render(std::vector<std::string>& pickedIdentifiers, EditorPage& editorPage);
    bool render(std::string& pickedIdentifier, EditorPage& editorPage);

private:
    AssetNode* getCurrentNode() const;

    bool isOpen = false;
    bool openRequested = false;
    std::string title = "Select Resource";

    AssetNode* rootNode = nullptr;
    AssetNode* currentNode = nullptr;
    std::vector<AssetNode*> navStack;

    Cube::ResourceType allowedType = Cube::ResourceType::Unknown;

    std::vector<std::string> selectedIdentifiers;
    std::unordered_set<std::string> selectedIdentifierSet;
    int selectionAnchorIndex = -1;
    std::unordered_set<std::string> expandedTextures;
};
