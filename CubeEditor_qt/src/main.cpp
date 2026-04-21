#include <QApplication>
#include <QMainWindow>

#include "Views/GuidancePage.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("CubeEditor (Qt) - Guidance");
    window.resize(980, 620);
    window.setCentralWidget(new GuidancePage(&window));

    window.show();
    return app.exec();
}