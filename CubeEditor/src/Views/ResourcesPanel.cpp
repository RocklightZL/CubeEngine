#include "ResourcesPanel.h"

#include "../Project.h"
#include "../App/EditorApp.h"
#include "../Scene/TextureMetadata.h"
#include "../Utils/ImGuiExternal.h"
#include "Cube/Core/Log.h"
#include "Cube/Renderer/Renderer.h"
#include "Cube/Resource/ResourceManager.h"
#include "Cube/UI/FileDialog.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include <memory>
#include <stack>

extern Cube::Project* proj;
extern Cube::EditorApp* app;

namespace Cube {

    void ResourcesPanel::render(float deltaTime) {
        std::deque<std::shared_ptr<Node>>& resStack = proj->resStack;
        std::shared_ptr<Node> currentNode = resStack.back();
        std::shared_ptr<Node> toDelete = nullptr;
        static int showMode = 0; // 0: icon mode 1: list mode
        ImGui::Begin("Resources Panel");
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

        std::string path;
        for(const std::shared_ptr<Node>& n : resStack) path += (n->name + "/");
        float topHeight = ImGui::CalcTextSize(path.c_str()).y;

        ImGui::BeginChild("TopBar", ImVec2(ImGui::GetContentRegionAvail().x, topHeight));
        topHeight -= ImGui::GetStyle().FramePadding.x * 2;
        if(ImGui::ImageButton("back", app->icons["back.png"]->getId(), ImVec2(topHeight, topHeight), {0, 1}, {1, 0})) {
            if(resStack.size() > 1) resStack.pop_back();
        }
        ImGui::SameLine();
        ImGui::Text(path.c_str());
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - topHeight - ImGui::GetStyle().FramePadding.x * 2);
        if(showMode == 0) {
            if(ImGui::ImageButton("iconMode", app->icons["icon_mode.png"]->getId(), ImVec2(topHeight, topHeight))) {
                showMode = 1;
            }
        }else if(showMode == 1) {
            if(ImGui::ImageButton("listMode", app->icons["list_mode.png"]->getId(), ImVec2(topHeight, topHeight))) {
                showMode = 0;
            }
        }
        ImGui::EndChild();

        static std::shared_ptr<Node> renamingNode;
        static char inputBuf[256] = {};
        static std::shared_ptr<ModalPopup> renamePopup = std::make_shared<ModalPopup>("Rename", [] {
            ImGui::Text("Name:");
            ImGui::InputText("##renameInput2", inputBuf, IM_ARRAYSIZE(inputBuf), ImGuiInputTextFlags_AutoSelectAll);
        }, [] {
            renamingNode->name = inputBuf;
            renamePopup->close();
        }, [] {
            if(renamingNode) renamingNode->isRenaming = false;
            renamingNode = nullptr;
        });

        ImGui::BeginChild("Content", ImGui::GetContentRegionAvail());
        float imageSize = 128.0f;
        for(auto& n : currentNode->children) {
            ImGui::PushID(&n);
            if(showMode == 0) {
                // static char inputBuf[256] = {};
                // static std::shared_ptr<ModalPopup> renamePopup = std::make_shared<ModalPopup>("Rename", [] {
                //     ImGui::Text("Name:");
                //     ImGui::InputText("##renameInput2", inputBuf, IM_ARRAYSIZE(inputBuf));
                // }, [&n] {
                //     n->isRenaming = false;
                //     n->name = inputBuf;
                // }, [&n] {
                //     memset(inputBuf, '\0', sizeof(inputBuf));
                //     n->isRenaming = false;
                // });

                ImGui::SameLine();
                if(ImGui::GetContentRegionAvail().x < imageSize) {
                    ImGui::NewLine();
                }
                IconTextButton(app->icons["directory.png"]->getId(), n->name.c_str(), ImVec2(imageSize, imageSize), {0, 1}, {1, 0});
                if(ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                    ImGui::OpenPopup("NodeRightButtonMenu");
                }
                if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    if(n->isGroup) {
                        resStack.push_back(n);
                    }
                }
            }else if(showMode == 1){
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
                            resStack.push_back(n);
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
                }
            }
            if(ImGui::BeginPopup("NodeRightButtonMenu")) {
                if(ImGui::MenuItem("Rename")) {
                    n->isRenaming = true;
                    n->justRenaming = true;
                    if(showMode == 0) {
                        renamingNode = n;
                        strcpy_s(inputBuf, n->name.c_str());
                        renamePopup->open();
                    }
                }
                if(ImGui::MenuItem("Delete")) {
                    toDelete = n;
                }
                ImGui::EndPopup();
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
                currentNode->children.push_back(n);
            }
            if(ImGui::MenuItem("Import Resources##2")) {
                importResources();
            }
            ImGui::EndPopup();
        }
        ImGui::EndChild();

        renamePopup->render();

        ImGui::PopStyleColor();
        ImGui::End();
        if(toDelete) {
            currentNode->children.erase(std::find(currentNode->children.begin(), currentNode->children.end(), toDelete));
        }

        ImGui::Begin("ResourcesPreview");

        ImGui::End();
    }

    void ResourcesPanel::importResources() {
        std::shared_ptr<Node> currentNode = proj->resStack.back();
        for(auto& path : FileDialog::openMultiFiles("Resources(.png.jpg)\0*.png;*.jpg\0", app->getWindow()->getWin32Window())) {
            std::string fileName = Utils::getFileName(path, true);
            if(!Utils::isFileInDirectory(path, proj->getConfig().resourcesDirectory)) {
                Utils::copyFile(path, proj->getConfig().resourcesDirectory + "/" + fileName);
            }
            if(std::find_if(currentNode->children.begin(), currentNode->children.end(), [fileName](std::shared_ptr<Node> x){ return x->name == fileName; }) == currentNode->children.end()){
                std::shared_ptr<Node> n = std::make_shared<Node>();
                n->isGroup = false;
                n->name = fileName;
                currentNode->children.push_back(n);
            } // TODO: 提醒用户不能添加重复资源
        }
    }

}  // namespace Cube