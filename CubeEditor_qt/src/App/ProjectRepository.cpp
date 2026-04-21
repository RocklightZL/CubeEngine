#include "ProjectRepository.h"

#include "Cube/Core/Log.h"

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
        CB_EDITOR_WARN("Skip adding recent project: empty project path.");
        return false;
    }

    m_recentProjects.removeAll(normalizedPath);
    m_recentProjects.prepend(normalizedPath);

    constexpr int kMaxRecentItems = 20;
    while(m_recentProjects.size() > kMaxRecentItems) {
        m_recentProjects.removeLast();
    }

    save();
    CB_EDITOR_INFO("Recent project added: {}", normalizedPath.toStdString());
    return true;
}

ProjectRepository::CreateProjectResult ProjectRepository::createProject(const QString& projectName, const QString& projectDir) {
    CreateProjectResult result;

    const QString cleanName = projectName.trimmed();
    const QString cleanDir = QDir::fromNativeSeparators(projectDir).trimmed();

    if(cleanName.isEmpty()) {
        result.error = "Project name cannot be empty.";
        CB_EDITOR_WARN("Create project failed: empty project name.");
        return result;
    }

    const QDir workspaceDir(cleanDir);
    if(!workspaceDir.exists()) {
        result.error = "Project location does not exist.";
        CB_EDITOR_WARN("Create project failed: project location does not exist: {}", cleanDir.toStdString());
        return result;
    }

    const QString projectRootPath = workspaceDir.filePath(cleanName);
    QDir projectRootDir(projectRootPath);

    if(projectRootDir.exists()) {
        result.error = "Project root directory already exists.";
        CB_EDITOR_WARN("Create project failed: project root already exists: {}", projectRootPath.toStdString());
        return result;
    }

    if(!workspaceDir.mkpath(cleanName)) {
        result.error = "Cannot create project root directory.";
        CB_EDITOR_ERROR("Create project failed: cannot create project root: {}", projectRootPath.toStdString());
        return result;
    }

    projectRootDir = QDir(projectRootPath);

    const QString cacheDirPath = projectRootDir.filePath(".cube");
    const QString scenesDirPath = projectRootDir.filePath("Scenes");
    const QString assetsDirPath = projectRootDir.filePath("Assets");

    if(!projectRootDir.mkpath(".cube") || !projectRootDir.mkpath("Scenes") || !projectRootDir.mkpath("Assets")) {
        result.error = "Cannot create required project directories.";
        CB_EDITOR_ERROR("Create project failed: cannot create required directories under {}", projectRootPath.toStdString());
        return result;
    }

    const QString projectFilePath = projectRootDir.filePath(cleanName + ".cbproj");

    QFile file(projectFilePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        result.error = "Cannot create project file.";
        CB_EDITOR_ERROR("Create project failed: cannot create file {}", projectFilePath.toStdString());
        return result;
    }

    QJsonObject projectObject;
    projectObject.insert("name", cleanName);
    projectObject.insert("root", QDir::fromNativeSeparators(projectRootPath));
    projectObject.insert("cacheDir", QDir::fromNativeSeparators(cacheDirPath));
    projectObject.insert("scenesDir", QDir::fromNativeSeparators(scenesDirPath));
    projectObject.insert("assetsDir", QDir::fromNativeSeparators(assetsDirPath));
    projectObject.insert("version", 1);

    QJsonObject rootObject;
    rootObject.insert("project", projectObject);

    const QJsonDocument doc(rootObject);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    addRecentProject(projectFilePath);

    result.ok = true;
    result.projectFilePath = QDir::fromNativeSeparators(projectFilePath);
    CB_EDITOR_INFO("Project created: {}", result.projectFilePath.toStdString());
    return result;
}

void ProjectRepository::load() {
    m_recentProjects.clear();

    QFile file(configFilePath());
    if(!file.exists()) {
        save();
        CB_EDITOR_INFO("Project cache initialized.");
        return;
    }

    if(!file.open(QIODevice::ReadOnly)) {
        CB_EDITOR_WARN("Cannot open project cache for reading.");
        return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if(!doc.isObject()) {
        CB_EDITOR_WARN("Invalid project cache format.");
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

    CB_EDITOR_INFO("Loaded {} cached projects.", static_cast<int>(m_recentProjects.size()));
}

void ProjectRepository::save() const {
    QFile file(configFilePath());
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        CB_EDITOR_WARN("Cannot open project cache for writing.");
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

    CB_EDITOR_INFO("Project cache saved. Count={}", static_cast<int>(m_recentProjects.size()));
}

QString ProjectRepository::configFilePath() {
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    if(configDir.isEmpty()) {
        configDir = QDir::homePath() + "/CubeEditor";
    }else{
        configDir += "/CubeEditor";
    }

    QDir dir(configDir);
    dir.mkpath(".");

    return dir.filePath("project_path_cache.json");
}
