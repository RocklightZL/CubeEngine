#pragma once

#include <QString>
#include <QStringList>

class ProjectRepository final {
public:
    struct CreateProjectResult {
        bool ok = false;
        QString projectFilePath;
        QString error;
    };

    ProjectRepository();

    QStringList recentProjects() const;
    bool addRecentProject(const QString& projectFilePath);

    CreateProjectResult createProject(const QString& projectName, const QString& projectDir);

private:
    void load();
    void save() const;
    static QString configFilePath();

private:
    QStringList m_recentProjects;
};
