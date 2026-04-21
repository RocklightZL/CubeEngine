#pragma once

#include <functional>
#include <memory>

#include <QWidget>

class QListWidget;
class QListWidgetItem;
class ProjectRepository;

class GuidancePage final : public QWidget {
public:
    explicit GuidancePage(std::shared_ptr<ProjectRepository> repository,
                          std::function<void(const QString&)> openEditorCallback,
                          QWidget* parent = nullptr);

private:
    QWidget* buildLeftPanel();
    QWidget* buildRightPanel();
    void reloadRecentProjects();

    void onCreateProject();
    void onOpenProject();
    void onRecentProjectActivated(QListWidgetItem* item);

private:
    std::shared_ptr<ProjectRepository> m_repository;
    std::function<void(const QString&)> m_openEditorCallback;
    QListWidget* m_recentList = nullptr;
};
