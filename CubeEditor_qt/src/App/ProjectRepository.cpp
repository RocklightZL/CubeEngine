#include "ProjectRepository.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

ProjectRepository::ProjectRepository() {
    load();
}

QStringList ProjectRepository::recentProjects() const {
    return m_recentProjects;
}

bool ProjectRepository::addRecentProject(const QString& projectFilePath) {
    const QString normalizedPath = QDir::fromNativeSeparators(projectFilePath).trimmed();
    if(normalizedPath.isEmpty()) {
        return false;
    }

    m_recentProjects.removeAll(normalizedPath);
    m_recentProjects.prepend(normalizedPath);

    constexpr int kMaxRecentItems = 20;
    while(m_recentProjects.size() > kMaxRecentItems) {
        m_recentProjects.removeLast();
    }

    save();
    return true;
}

ProjectRepository::CreateProjectResult ProjectRepository::createProject(const QString& projectName, const QString& projectDir) {
    CreateProjectResult result;

    const QString cleanName = projectName.trimmed();
    const QString cleanDir = QDir::fromNativeSeparators(projectDir).trimmed();

    if(cleanName.isEmpty()) {
        result.error = "项目名不能为空";
        return result;
    }

    QDir dir(cleanDir);
    if(!dir.exists()) {
        result.error = "项目路径不存在";
        return result;
    }

    const QString projectFilePath = dir.filePath(cleanName + ".cbproj");
    if(QFileInfo::exists(projectFilePath)) {
        result.error = "项目文件已存在";
        return result;
    }

    QFile file(projectFilePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        result.error = "无法创建项目文件";
        return result;
    }

    QJsonObject projectObject;
    projectObject.insert("name", cleanName);
    projectObject.insert("path", cleanDir);

    QJsonObject rootObject;
    rootObject.insert("project", projectObject);

    const QJsonDocument doc(rootObject);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    addRecentProject(projectFilePath);

    result.ok = true;
    result.projectFilePath = QDir::fromNativeSeparators(projectFilePath);
    return result;
}

void ProjectRepository::load() {
    m_recentProjects.clear();

    QFile file(configFilePath());
    if(!file.exists()) {
        save();
        return;
    }

    if(!file.open(QIODevice::ReadOnly)) {
        return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if(!doc.isObject()) {
        return;
    }

    const QJsonArray projectsArray = doc.object().value("projectsPathCache").toArray();
    for(const QJsonValue& value : projectsArray) {
        if(value.isString()) {
            const QString path = QDir::fromNativeSeparators(value.toString());
            if(!path.isEmpty()) {
                m_recentProjects.push_back(path);
            }
        }
    }
}

void ProjectRepository::save() const {
    QFile file(configFilePath());
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return;
    }

    QJsonArray projectsArray;
    for(const QString& path : m_recentProjects) {
        projectsArray.append(path);
    }

    QJsonObject rootObject;
    rootObject.insert("projectsPathCache", projectsArray);

    const QJsonDocument doc(rootObject);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}

QString ProjectRepository::configFilePath() {
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    if(configDir.isEmpty()) {
        configDir = QDir::homePath() + "/.cube_editor_qt";
    }

    QDir dir(configDir);
    dir.mkpath(".");

    return dir.filePath("project_path_cache.json");
}
