#include "ResourcesPanel.h"

#include "../Project.h"
#include "../App/EditorApp.h"
#include "Cube/Core/Log.h"
#include "Cube/Resource/ResourceManager.h"
#include "Cube/UI/FileDialog.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include <memory>

extern Cube::Project* proj;
extern Cube::EditorApp* app;

namespace Cube {

    ResourcesPanel::ResourcesPanel() {
        dirIcon = ResourceManager::getInstance().load<Texture2D>("assets/icons/Directory.png");
        fileIcon = ResourceManager::getInstance().load<Texture2D>("assets/icons/File.png");
    }

    ResourcesPanel::~ResourcesPanel() {
        ResourceManager::getInstance().release(dirIcon);
        ResourceManager::getInstance().release(fileIcon);
    }

    void ResourcesPanel::render(float deltaTime) {
        std::shared_ptr<Node> currentNode = proj->currentNode;
        ImGui::Begin("Resources Panel");
        if(!currentNode->parent.expired()) {
            if(ImGui::Button("<<<")) {
                proj->currentNode = currentNode->parent.lock();
            }
        }
        for(auto& n : currentNode->children) {
            ImGui::PushID(&n);
            if(n->isRenaming) {
                if(n->justRenaming) {
                    ImGui::SetKeyboardFocusHere(0);
                    ImGui::SetActiveID(ImGui::GetID("##renameInput"), ImGui::GetCurrentWindow());
                    n->justRenaming = false;
                }
                char renameBuf[256] = {};
                strcpy_s(renameBuf, n->name.c_str());
                if(ImGui::InputText("##renameInput", renameBuf, IM_ARRAYSIZE(renameBuf), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
                    n->name = renameBuf;
                    n->isRenaming = false; // TODO: 名称合法性检测
                }

                if(!ImGui::IsItemActive() && ImGui::IsMouseClicked(0)) {
                    n->name = renameBuf;
                    n->isRenaming = false;
                }
            } else {
                if(n->isGroup) {
                    ImGui::Selectable(n->name.c_str());
                    if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                        proj->currentNode = n;
                    }
                } else {
                    ImGui::Selectable(n->name.c_str());
                    if(ImGui::BeginDragDropSource()) {
                        std::string texturePath = proj->getConfig().resourcesDirectory + "/" + n->name;
                        ImGui::Text(texturePath.c_str());
                        ImGui::SetDragDropPayload("TexturePath", texturePath.c_str(), texturePath.size() + 1);
                        ImGui::EndDragDropSource();
                    }
                }
                if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)) {
                    ImGui::OpenPopup("NodeRightButtonMenu");
                }
                if(ImGui::BeginPopup("NodeRightButtonMenu")) {
                    if(ImGui::MenuItem("Rename")) {
                        n->isRenaming = true;
                        n->justRenaming = true;
                    }
                    if(ImGui::MenuItem("Delete")) {
                    }
                    ImGui::EndPopup();
                }
            }
            ImGui::PopID();
        }
        if(ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1) && !ImGui::IsAnyItemHovered()) {
            ImGui::OpenPopup("RightButtonMenu");
        }
        if(ImGui::BeginPopup("RightButtonMenu")) {
            if(ImGui::MenuItem("New Group")) {
                auto n = std::make_shared<Node>();
                n->name = "NewGroup";
                n->isGroup = true;
                n->isRenaming = true;
                n->justRenaming = true;
                n->parent = currentNode;
                currentNode->children.push_back(n);
            }
            if(ImGui::MenuItem("Import Resources##2")) {
                importResources();
            }
            ImGui::EndPopup();
        }
        ImGui::End();
    }

    void ResourcesPanel::importResources() {
        for(auto& path : FileDialog::openMultiFiles("Resources(.png.jpg)\0*.png;*.jpg\0", app->getWindow()->getWin32Window())) {
            std::string fileName = Utils::getFileName(path, true);
            if(!Utils::isFileInDirectory(path, proj->getConfig().resourcesDirectory)) {
                Utils::copyFile(path, proj->getConfig().resourcesDirectory + "/" + fileName);
            }
            if(std::find_if(proj->currentNode->children.begin(), proj->currentNode->children.end(), [fileName](std::shared_ptr<Node> x){ return x->name == fileName; }) == proj->currentNode->children.end()){
                std::shared_ptr<Node> n = std::make_shared<Node>();
                n->isGroup = false;
                n->name = fileName;
                proj->currentNode->children.push_back(n);
            } // TODO: 提醒用户不能添加重复资源
        }
    }
}  // namespace Cube