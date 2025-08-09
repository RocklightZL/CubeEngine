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
        std::shared_ptr<Node> toDelete = nullptr;
        ImGui::Begin("Resources Panel");
        if(!currentNode->parent.expired()) {
            if(ImGui::Button("<<<")) {
                proj->currentNode = currentNode->parent.lock();
            }
        }

        static bool showSplitPopup = false;
        static Texture2D* splitTexture = nullptr;
        static std::shared_ptr<TextureMetadata> metadata;
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
                        toDelete = n;
                    }
                    std::string s = Utils::getFileSuffix(n->name);
                    if(s == ".png" || s == ".jpg") {
                        if(ImGui::MenuItem("Split")) {
                            if(splitTexture) ResourceManager::getInstance().release(splitTexture->getFilePath());
                            splitTexture = ResourceManager::getInstance().load<Texture2D>(proj->getConfig().resourcesDirectory + "/" + n->name)->data;
                            if(Utils::isFileExists(splitTexture->getFilePath() + ".meta")) {
                                metadata = std::make_shared<TextureMetadata>(splitTexture->getFilePath() + ".meta");
                            }else {
                                metadata = std::make_shared<TextureMetadata>(glm::vec2(splitTexture->getWidth(), splitTexture->getHeight()));
                            }
                            showSplitPopup = true;
                        }
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

        // SplitPopup
        if(showSplitPopup) {
            ImGui::OpenPopup("SplitPopup");
        }
        if(ImGui::BeginPopupModal("SplitPopup", &showSplitPopup, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 windowPos = ImGui::GetWindowPos();
            float titleBarHeight = ImGui::GetCurrentWindow()->TitleBarHeight;
            ImVec2 padding = ImGui::GetStyle().WindowPadding;
            ImVec2 contentPos(windowPos.x + padding.x, windowPos.y + titleBarHeight + padding.y);

            ImGui::Image(splitTexture->getId(), {(float)splitTexture->getWidth(), (float)splitTexture->getHeight()}, ImVec2(0, 1), ImVec2(1, 0));
            drawList->PathClear();
            drawList->PathLineTo(ImVec2(contentPos.x, contentPos.y));
            drawList->PathLineTo(ImVec2(contentPos.x, contentPos.y + splitTexture->getHeight()));
            drawList->PathLineTo(ImVec2(contentPos.x + splitTexture->getWidth(), contentPos.y + splitTexture->getHeight()));
            drawList->PathLineTo(ImVec2(contentPos.x + splitTexture->getWidth(), contentPos.y));
            drawList->PathStroke(IM_COL32(255, 255, 255, 255), ImDrawFlags_Closed, 2);

            ImGui::SameLine();

            ImGui::BeginGroup();
            static int selected = 0;
            constexpr const char* items[] = {"Grid equal division", "Custom division"};
            ImGui::Text("Division mode:");
            ImGui::Combo("##DivisionMode", &selected, items, IM_ARRAYSIZE(items));
            if(selected == 0) {
                static int rowNum = 1;
                static int columnNum = 1;
                ImGui::Text("Number of rows:");
                ImGui::InputInt("##RowNum", &rowNum);
                rowNum = ImClamp(rowNum, 1, 1000);
                ImGui::Text("Number of columns:");
                ImGui::InputInt("##ColumNum", &columnNum);
                columnNum = ImClamp(columnNum, 1, 1000);

                float width = (float)splitTexture->getWidth() / columnNum;
                float height = (float)splitTexture->getHeight() / rowNum;
                for(int i = 1; i < columnNum; ++i) {
                    addDashLine(drawList, contentPos + ImVec2(width * i, 0), contentPos + ImVec2(width * i, splitTexture->getHeight()), ImColor(255, 0, 0, 255));
                }
                for(int i = 1; i < rowNum; ++i) {
                    addDashLine(drawList, contentPos + ImVec2(0, height * i), contentPos + ImVec2(splitTexture->getWidth(), height * i), ImColor(255, 0, 0, 255));
                }
            }else if(selected == 1) {
                
            }
            ImGui::EndGroup();

            if(ImGui::Button("OK")) {
                showSplitPopup = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if(ImGui::Button("Cancel")) {
                showSplitPopup = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
        // SplitPopup end
        ImGui::End();
        if(toDelete) {
            currentNode->children.erase(std::find(currentNode->children.begin(), currentNode->children.end(), toDelete));
        }

        ImGui::Begin("ResourcesPreview");

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