#include <QApplication>
#include <QMainWindow>

#include <memory>

#include "App/EditorWindow.h"
#include "App/ProjectRepository.h"
#include "Views/GuidancePage.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("CubeEditor (Qt) - Guidance");
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