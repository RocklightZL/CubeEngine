#include <QApplication>
#include <QDialog>
#include <QFileDialog>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QSpacerItem>
#include <QStringList>
#include <QVBoxLayout>
#include <QWidget>
#include <utility>

class NewProjectDialog final : public QDialog {
public:
    explicit NewProjectDialog(QWidget* parent = nullptr)
        : QDialog(parent) {
        setWindowTitle("New Project");
        setModal(true);
        resize(520, 220);

        auto* root = new QVBoxLayout(this);
        root->setContentsMargins(18, 18, 18, 18);
        root->setSpacing(12);

        auto* nameLabel = new QLabel("Project Name", this);
        m_nameEdit = new QLineEdit(this);
        m_nameEdit->setPlaceholderText("Input project name...");

        auto* pathLabel = new QLabel("Project Path", this);
        auto* pathRow = new QHBoxLayout();
        m_pathEdit = new QLineEdit(this);
        m_pathEdit->setPlaceholderText("Choose project directory...");
        auto* browseButton = new QPushButton("Browse", this);
        browseButton->setFixedWidth(92);

        pathRow->addWidget(m_pathEdit, 1);
        pathRow->addWidget(browseButton);

        auto* actions = new QHBoxLayout();
        actions->addStretch(1);
        auto* cancelButton = new QPushButton("Cancel", this);
        auto* createButton = new QPushButton("Create", this);
        createButton->setProperty("role", "primary");
        actions->addWidget(cancelButton);
        actions->addWidget(createButton);

        root->addWidget(nameLabel);
        root->addWidget(m_nameEdit);
        root->addWidget(pathLabel);
        root->addLayout(pathRow);
        root->addStretch(1);
        root->addLayout(actions);

        connect(browseButton, &QPushButton::clicked, this, [this] {
            const QString dir = QFileDialog::getExistingDirectory(this, "Select Project Directory", m_pathEdit->text());
            if(!dir.isEmpty()) {
                m_pathEdit->setText(dir);
            }
        });
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
        connect(createButton, &QPushButton::clicked, this, [this] {
            QMessageBox::information(this, "UI Prototype", "Create logic is not connected yet.");
        });
    }

private:
    QLineEdit* m_nameEdit = nullptr;
    QLineEdit* m_pathEdit = nullptr;
};

class GuidancePage final : public QWidget {
public:
    explicit GuidancePage(QWidget* parent = nullptr)
        : QWidget(parent) {
        auto* root = new QHBoxLayout(this);
        root->setContentsMargins(24, 24, 24, 24);
        root->setSpacing(24);

        auto* leftPane = buildRecentProjectsPane(this);
        auto* rightPane = buildEntryPane(this);

        root->addWidget(leftPane, 1);
        root->addWidget(rightPane, 2);

        setStyleSheet(R"(
            QWidget {
                background: #1e2228;
                color: #e8edf2;
                font-size: 13px;
            }
            QGroupBox {
                border: 1px solid #303843;
                border-radius: 10px;
                margin-top: 14px;
                padding-top: 12px;
                font-weight: 600;
                color: #dbe3ea;
            }
            QGroupBox::title {
                subcontrol-origin: margin;
                left: 10px;
                padding: 0 6px;
            }
            QListWidget {
                border: 1px solid #303843;
                border-radius: 8px;
                background: #171b21;
                padding: 6px;
                outline: none;
            }
            QListWidget::item {
                padding: 8px 10px;
                border-radius: 6px;
            }
            QListWidget::item:selected {
                background: #2d4f73;
                color: #ffffff;
            }
            QPushButton {
                border: 1px solid #394656;
                border-radius: 8px;
                background: #283341;
                color: #e8edf2;
                padding: 8px 14px;
            }
            QPushButton:hover {
                background: #314154;
            }
            QPushButton:pressed {
                background: #23303d;
            }
            QPushButton[role="primary"] {
                border: 1px solid #3b7ec3;
                background: #2d6ead;
                color: #ffffff;
            }
            QPushButton[role="primary"]:hover {
                background: #367dc0;
            }
            QFrame#entryCard {
                border: 1px solid #313a46;
                border-radius: 12px;
                background: #171c23;
            }
            QLabel#entryIcon {
                font-size: 34px;
                font-weight: 700;
                color: #8fb7df;
            }
            QLabel#entryTitle {
                font-size: 18px;
                font-weight: 650;
            }
            QLabel#entryDesc {
                color: #a4b2c1;
            }
            QLineEdit {
                border: 1px solid #394656;
                border-radius: 8px;
                padding: 7px 10px;
                background: #11161c;
                color: #e8edf2;
            }
        )");
    }

private:
    static QWidget* buildRecentProjectsPane(QWidget* parent) {
        auto* group = new QGroupBox("Recent Projects", parent);
        auto* layout = new QVBoxLayout(group);
        layout->setContentsMargins(12, 16, 12, 12);

        auto* list = new QListWidget(group);
        list->addItems(QStringList{
            "D:/mycode/vsProject/CubeEngine/Test/CubeEditorProject/Test01/Test01.cbproj",
            "D:/mycode/vsProject/CubeEngine/Sandbox/assets/scenes/Demo.cbproj",
            "D:/mycode/vsProject/CubeEngine/Test/CubeEditorProject/Sample/Sample.cbproj"
        });

        auto* tip = new QLabel("Double click project item to open (logic not connected).", group);
        tip->setStyleSheet("color: #8f9aa7;");

        layout->addWidget(list, 1);
        layout->addWidget(tip);
        return group;
    }

    QWidget* buildEntryPane(QWidget* parent) {
        auto* wrapper = new QWidget(parent);
        auto* layout = new QVBoxLayout(wrapper);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(18);

        auto* title = new QLabel("Project Guidance", wrapper);
        title->setStyleSheet("font-size: 26px; font-weight: 700;");
        auto* subtitle = new QLabel("Create a new project or open an existing project file.", wrapper);
        subtitle->setStyleSheet("color: #98a6b6; font-size: 14px;");

        auto* cards = new QHBoxLayout();
        cards->setSpacing(18);
        cards->addWidget(buildEntryCard("+", "New Project", "Create a clean editor project scaffold.", "Create", [this] {
            NewProjectDialog dialog(this);
            dialog.exec();
        }));
        cards->addWidget(buildEntryCard("<>", "Open Project", "Open an existing .cbproj file.", "Open", [this] {
            QMessageBox::information(this, "UI Prototype", "Open logic is not connected yet.");
        }));

        layout->addStretch(1);
        layout->addWidget(title);
        layout->addWidget(subtitle);
        layout->addSpacing(8);
        layout->addLayout(cards);
        layout->addStretch(2);

        return wrapper;
    }

    template<typename Fn>
    static QFrame* buildEntryCard(const QString& iconText,
                                  const QString& titleText,
                                  const QString& descText,
                                  const QString& actionText,
                                  Fn&& callback) {
        auto* card = new QFrame();
        card->setObjectName("entryCard");
        card->setMinimumSize(290, 210);

        auto* layout = new QVBoxLayout(card);
        layout->setContentsMargins(18, 16, 18, 16);
        layout->setSpacing(8);

        auto* icon = new QLabel(iconText, card);
        icon->setObjectName("entryIcon");

        auto* title = new QLabel(titleText, card);
        title->setObjectName("entryTitle");

        auto* desc = new QLabel(descText, card);
        desc->setWordWrap(true);
        desc->setObjectName("entryDesc");

        auto* button = new QPushButton(actionText, card);
        button->setProperty("role", "primary");

        layout->addWidget(icon);
        layout->addWidget(title);
        layout->addWidget(desc);
        layout->addStretch(1);
        layout->addWidget(button);

        QObject::connect(button, &QPushButton::clicked, std::forward<Fn>(callback));
        return card;
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("CubeEditor (Qt) - Guidance");
    window.resize(1240, 760);
    window.setCentralWidget(new GuidancePage(&window));

    window.show();
    return app.exec();
}