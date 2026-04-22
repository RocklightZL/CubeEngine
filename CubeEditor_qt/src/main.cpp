#include <qapplication.h>
#include <qnamespace.h>
#include <QApplication>
#include <QMainWindow>

#include <memory>

#include "App/ProjectRepository.h"
#include "Cube/Core/Engine.h"
#include "Views/EditorWindow.h"
#include "Views/GuidancePage.h"

int main(int argc, char* argv[]) {
    Cube::Engine::init();
    QApplication app(argc, argv);
    app.setApplicationName("CubeEditor");
    QMainWindow window;
    window.setWindowTitle("CubeEditor - Guidance");
    window.resize(980, 620);

    auto repository = std::make_shared<ProjectRepository>();
    auto openEditor = [&window](const QString& projectPath) {
        auto* editorWindow = new EditorWindow(projectPath);
        editorWindow->setAttribute(Qt::WA_DeleteOnClose);
        editorWindow->show();
        window.close();
    };

    window.setCentralWidget(new GuidancePage(repository, openEditor, &window));

    window.show();
    return app.exec();
}