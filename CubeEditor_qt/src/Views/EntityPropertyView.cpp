#include "EntityPropertyView.h"

#include "../Utils/NumericDragEdit.h"

#include "Cube/Reflection/Any.h"
#include "Cube/Reflection/Class.h"
#include "Cube/Reflection/ClassRegistry.h"
#include "Cube/Renderer/Color.h"
#include "Cube/Renderer/TextureRegion.h"
#include "Cube/Scene/Camera2D.h"
#include "Cube/Scene/Component.h"
#include "Cube/Scene/Entity.h"
#include "Cube/Scene/Scene.h"
#include "Cube/Scene/SpriteRender.h"
#include "Cube/Scene/Transform.h"
#include "Cube/Animation/Animation.h"

#include <QCheckBox>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>

#include <glm/glm.hpp>

#include <utility>

namespace {
bool isIntegerType(Cube::TypeID typeID) {
    return typeID == Cube::getTypeID<int8_t>() ||
           typeID == Cube::getTypeID<uint8_t>() ||
           typeID == Cube::getTypeID<int16_t>() ||
           typeID == Cube::getTypeID<uint16_t>() ||
           typeID == Cube::getTypeID<int32_t>() ||
           typeID == Cube::getTypeID<uint32_t>() ||
           typeID == Cube::getTypeID<int64_t>() ||
           typeID == Cube::getTypeID<uint64_t>();
}

bool isFloatingType(Cube::TypeID typeID) {
    return typeID == Cube::getTypeID<float>() || typeID == Cube::getTypeID<double>();
}

bool isNumericType(Cube::TypeID typeID) {
    return isIntegerType(typeID) || isFloatingType(typeID);
}

}

EntityPropertyView::EntityPropertyView(QWidget* parent)
    : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);

    m_entityNameLabel = new QLabel(this);
    m_entityNameLabel->setObjectName("entityNameLabel");
    layout->addWidget(m_entityNameLabel);

    m_tree = new QTreeWidget(this);
    m_tree->setObjectName("entityPropertyTree");
    m_tree->setColumnCount(2);
    m_tree->setHeaderHidden(true);
    m_tree->setRootIsDecorated(true);
    m_tree->setAlternatingRowColors(false);
    m_tree->setExpandsOnDoubleClick(false);
    m_tree->setContextMenuPolicy(Qt::CustomContextMenu);

    const auto toggleExpanded = [](QTreeWidgetItem* item) {
        if(!item || item->childCount() == 0) {
            return;
        }
        item->setExpanded(!item->isExpanded());
    };

    connect(m_tree, &QTreeWidget::itemClicked, this, [toggleExpanded](QTreeWidgetItem* item, int) {
        toggleExpanded(item);
    });
    connect(m_tree, &QTreeWidget::itemDoubleClicked, this, [toggleExpanded](QTreeWidgetItem* item, int) {
        toggleExpanded(item);
    });
    connect(m_tree, &QTreeWidget::customContextMenuRequested, this, [this](const QPoint& pos) {
        showAddComponentMenu(m_tree->viewport()->mapToGlobal(pos));
    });

    layout->addWidget(m_tree);

    m_addComponentButton = new QPushButton("Add Component", this);
    m_addComponentButton->setObjectName("addComponentButton");
    connect(m_addComponentButton, &QPushButton::clicked, this, [this] {
        showAddComponentMenu(m_addComponentButton->mapToGlobal(QPoint(0, m_addComponentButton->height())));
    });
    layout->addWidget(m_addComponentButton);

    refresh();
}

void EntityPropertyView::setSelection(Cube::Scene* scene, Cube::Entity* entity) {
    m_scene = scene;
    m_entity = entity;
    refresh();
}

void EntityPropertyView::setSceneDirtyCallback(std::function<void(Cube::Scene*)> callback) {
    m_sceneDirtyCallback = std::move(callback);
}

void EntityPropertyView::notifySceneDirty() {
    if(m_sceneDirtyCallback && m_scene) {
        m_sceneDirtyCallback(m_scene);
    }
}

void EntityPropertyView::refresh() {
    m_tree->clear();

    if(!m_scene || !m_entity) {
        m_entityNameLabel->setText("No entity selected.");
        if(m_addComponentButton) {
            m_addComponentButton->setEnabled(false);
        }
        return;
    }

    m_entityNameLabel->setText(QString::fromStdString(m_entity->getName()));
    if(m_addComponentButton) {
        m_addComponentButton->setEnabled(true);
    }

    appendTransform();

    const auto& components = m_entity->getComponents();
    for(const auto& component : components) {
        if(!component) {
            continue;
        }

        Cube::Class* classInfo = Cube::ClassRegistry::get().getClass(component->getType());
        const QString componentName = classInfo ? QString::fromStdString(classInfo->getName()) : QStringLiteral("UnknownComponent");
        appendComponent(componentName, component.get(), classInfo);
    }

    m_tree->expandAll();
    m_tree->resizeColumnToContents(0);
}

void EntityPropertyView::showAddComponentMenu(const QPoint& globalPos) {
    if(!m_entity || !m_scene) {
        return;
    }

    QMenu menu(this);
    auto* addMenu = menu.addMenu("Add Component");

    auto* cameraAction = addMenu->addAction("Camera2D");
    cameraAction->setEnabled(!m_entity->hasComponent<Cube::Camera2D>());

    auto* spriteAction = addMenu->addAction("SpriteRender");
    spriteAction->setEnabled(!m_entity->hasComponent<Cube::SpriteRender>());

    auto* animationAction = addMenu->addAction("Animation");
    animationAction->setEnabled(!m_entity->hasComponent<Cube::Animation>());

    QAction* picked = menu.exec(globalPos);
    if(!picked) {
        return;
    }

    if(picked == cameraAction) {
        addComponentByName("Camera2D");
    } else if(picked == spriteAction) {
        addComponentByName("SpriteRender");
    } else if(picked == animationAction) {
        addComponentByName("Animation");
    }
}

void EntityPropertyView::addComponentByName(const QString& componentName) {
    if(!m_entity || !m_scene) {
        return;
    }

    if(componentName == "Camera2D") {
        m_entity->addComponent<Cube::Camera2D>();
    } else if(componentName == "SpriteRender") {
        m_entity->addComponent<Cube::SpriteRender>();
    } else if(componentName == "Animation") {
        m_entity->addComponent<Cube::Animation>();
    } else {
        return;
    }

    // Flush entity pending add queue so the new component becomes visible immediately.
    m_scene->update(0.0f);
    notifySceneDirty();
    refresh();
}

void EntityPropertyView::appendTransform() {
    auto* transformItem = new QTreeWidgetItem(m_tree);
    transformItem->setText(0, "Transform");
    transformItem->setExpanded(true);

    auto& transform = m_entity->getTransform();

    auto createFloatEditor = [this](QTreeWidgetItem* item, double value, const std::function<void(double)>& onCommit) {
        auto* editor = new NumericDragEdit(false, m_tree);
        editor->setValue(value);
        connect(editor, &NumericDragEdit::valueCommitted, this, [onCommit](double v) {
            onCommit(v);
        });
        m_tree->setItemWidget(item, 1, editor);
    };

    auto createVec2Editor = [this](QTreeWidgetItem* item, const glm::vec2& value, const std::function<void(const glm::vec2&)>& onCommit) {
        auto* row = new QWidget(m_tree);
        auto* rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        rowLayout->setSpacing(4);

        auto* xEdit = new NumericDragEdit(false, row);
        auto* yEdit = new NumericDragEdit(false, row);
        xEdit->setValue(value.x);
        yEdit->setValue(value.y);

        rowLayout->addWidget(xEdit);
        rowLayout->addWidget(yEdit);

        auto commit = [onCommit, xEdit, yEdit]() {
            onCommit(glm::vec2(static_cast<float>(xEdit->value()), static_cast<float>(yEdit->value())));
        };
        connect(xEdit, &NumericDragEdit::valueCommitted, this, [commit](double) { commit(); });
        connect(yEdit, &NumericDragEdit::valueCommitted, this, [commit](double) { commit(); });

        m_tree->setItemWidget(item, 1, row);
    };

    const glm::vec2 position = transform.getPosition();
    auto* positionItem = new QTreeWidgetItem(transformItem);
    positionItem->setText(0, "Position");
    createVec2Editor(positionItem, position, [this](const glm::vec2& v) {
        m_entity->getTransform().setPosition(v);
        notifySceneDirty();
    });

    auto* rotItem = new QTreeWidgetItem(transformItem);
    rotItem->setText(0, "Rotation");
    createFloatEditor(rotItem, transform.getRotation(), [this](double v) {
        m_entity->getTransform().setRotation(static_cast<float>(v));
        notifySceneDirty();
    });

    const glm::vec2 scale = transform.getScale();
    auto* scaleItem = new QTreeWidgetItem(transformItem);
    scaleItem->setText(0, "Scale");
    createVec2Editor(scaleItem, scale, [this](const glm::vec2& v) {
        m_entity->getTransform().setScale(v);
        notifySceneDirty();
    });
}

void EntityPropertyView::appendComponent(const QString& componentName,
                                         void* componentData,
                                         const Cube::Class* classInfo) {
    auto* componentItem = new QTreeWidgetItem(m_tree);
    componentItem->setText(0, componentName);
    componentItem->setExpanded(true);

    if(!classInfo) {
        auto* unsupported = new QTreeWidgetItem(componentItem);
        unsupported->setText(0, "<Reflection not registered>");
        return;
    }

    const auto properties = classInfo->getAllProperties();
    if(properties.empty()) {
        auto* empty = new QTreeWidgetItem(componentItem);
        empty->setText(0, "<No reflected fields>");
        return;
    }

    for(const Cube::Property* property : properties) {
        if(!property) {
            continue;
        }
        Cube::Any value = property->getValue(componentData);
        appendPropertyFlat(componentItem, componentData, property, value);
    }
}

void EntityPropertyView::appendPropertyFlat(QTreeWidgetItem* parent,
                                            void* componentData,
                                            const Cube::Property* property,
                                            const Cube::Any& value) {
    if(!property) {
        return;
    }

    auto* item = new QTreeWidgetItem(parent);
    item->setText(0, QString::fromStdString(property->getName()));

    const Cube::TypeID typeID = property->getTypeID();

    if(isNumericType(typeID)) {
        auto* editor = new NumericDragEdit(isIntegerType(typeID), m_tree);
        if(isFloatingType(typeID)) {
            if(typeID == Cube::getTypeID<float>()) {
                editor->setValue(value.as<float>());
            } else {
                editor->setValue(value.as<double>());
            }
        } else {
            if(typeID == Cube::getTypeID<int8_t>()) {
                editor->setValue(value.as<int8_t>());
            } else if(typeID == Cube::getTypeID<uint8_t>()) {
                editor->setValue(value.as<uint8_t>());
            } else if(typeID == Cube::getTypeID<int16_t>()) {
                editor->setValue(value.as<int16_t>());
            } else if(typeID == Cube::getTypeID<uint16_t>()) {
                editor->setValue(value.as<uint16_t>());
            } else if(typeID == Cube::getTypeID<int32_t>()) {
                editor->setValue(value.as<int32_t>());
            } else if(typeID == Cube::getTypeID<uint32_t>()) {
                editor->setValue(value.as<uint32_t>());
            } else if(typeID == Cube::getTypeID<int64_t>()) {
                editor->setValue(static_cast<double>(value.as<int64_t>()));
            } else if(typeID == Cube::getTypeID<uint64_t>()) {
                editor->setValue(static_cast<double>(value.as<uint64_t>()));
            }
        }

        connect(editor, &NumericDragEdit::valueCommitted, this, [this, property, componentData, typeID](double v) {
            if(typeID == Cube::getTypeID<float>()) {
                property->setValue(componentData, Cube::Any(static_cast<float>(v)));
            } else if(typeID == Cube::getTypeID<double>()) {
                property->setValue(componentData, Cube::Any(static_cast<double>(v)));
            } else if(typeID == Cube::getTypeID<int8_t>()) {
                property->setValue(componentData, Cube::Any(static_cast<int8_t>(v)));
            } else if(typeID == Cube::getTypeID<uint8_t>()) {
                property->setValue(componentData, Cube::Any(static_cast<uint8_t>(v)));
            } else if(typeID == Cube::getTypeID<int16_t>()) {
                property->setValue(componentData, Cube::Any(static_cast<int16_t>(v)));
            } else if(typeID == Cube::getTypeID<uint16_t>()) {
                property->setValue(componentData, Cube::Any(static_cast<uint16_t>(v)));
            } else if(typeID == Cube::getTypeID<int32_t>()) {
                property->setValue(componentData, Cube::Any(static_cast<int32_t>(v)));
            } else if(typeID == Cube::getTypeID<uint32_t>()) {
                property->setValue(componentData, Cube::Any(static_cast<uint32_t>(v)));
            } else if(typeID == Cube::getTypeID<int64_t>()) {
                property->setValue(componentData, Cube::Any(static_cast<int64_t>(v)));
            } else if(typeID == Cube::getTypeID<uint64_t>()) {
                property->setValue(componentData, Cube::Any(static_cast<uint64_t>(v)));
            }
            notifySceneDirty();
        });

        m_tree->setItemWidget(item, 1, editor);
        return;
    }

    if(typeID == Cube::getTypeID<bool>()) {
        auto* check = new QCheckBox(m_tree);
        check->setChecked(value.as<bool>());
        connect(check, &QCheckBox::toggled, this, [this, property, componentData](bool checked) {
            property->setValue(componentData, Cube::Any(checked));
            notifySceneDirty();
        });
        m_tree->setItemWidget(item, 1, check);
        return;
    }

    if(typeID == Cube::getTypeID<std::string>()) {
        auto* edit = new QLineEdit(QString::fromStdString(value.as<std::string>()), m_tree);
        edit->setFrame(false);
        connect(edit, &QLineEdit::editingFinished, this, [this, property, componentData, edit] {
            property->setValue(componentData, Cube::Any(edit->text().toStdString()));
            notifySceneDirty();
        });
        m_tree->setItemWidget(item, 1, edit);
        return;
    }

    if(typeID == Cube::getTypeID<glm::vec1>()) {
        auto* editor = new NumericDragEdit(false, m_tree);
        editor->setValue(value.as<glm::vec1>().x);
        connect(editor, &NumericDragEdit::valueCommitted, this, [this, property, componentData](double v) {
            glm::vec1 next(static_cast<float>(v));
            property->setValue(componentData, Cube::Any(next));
            notifySceneDirty();
        });
        m_tree->setItemWidget(item, 1, editor);
        return;
    }

    if(typeID == Cube::getTypeID<glm::vec2>()) {
        const glm::vec2 vec = value.as<glm::vec2>();
        auto* row = new QWidget(m_tree);
        auto* rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        rowLayout->setSpacing(4);

        auto* xEdit = new NumericDragEdit(false, row);
        auto* yEdit = new NumericDragEdit(false, row);
        xEdit->setValue(vec.x);
        yEdit->setValue(vec.y);
        rowLayout->addWidget(xEdit);
        rowLayout->addWidget(yEdit);

        auto commit = [this, property, componentData, xEdit, yEdit]() {
            glm::vec2 next(static_cast<float>(xEdit->value()), static_cast<float>(yEdit->value()));
            property->setValue(componentData, Cube::Any(next));
            notifySceneDirty();
        };
        connect(xEdit, &NumericDragEdit::valueCommitted, this, [commit](double) { commit(); });
        connect(yEdit, &NumericDragEdit::valueCommitted, this, [commit](double) { commit(); });

        m_tree->setItemWidget(item, 1, row);
        return;
    }

    if(typeID == Cube::getTypeID<glm::vec3>()) {
        const glm::vec3 vec = value.as<glm::vec3>();
        auto* row = new QWidget(m_tree);
        auto* rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        rowLayout->setSpacing(4);

        auto* xEdit = new NumericDragEdit(false, row);
        auto* yEdit = new NumericDragEdit(false, row);
        auto* zEdit = new NumericDragEdit(false, row);
        xEdit->setValue(vec.x);
        yEdit->setValue(vec.y);
        zEdit->setValue(vec.z);
        rowLayout->addWidget(xEdit);
        rowLayout->addWidget(yEdit);
        rowLayout->addWidget(zEdit);

        auto commit = [this, property, componentData, xEdit, yEdit, zEdit]() {
            glm::vec3 next(static_cast<float>(xEdit->value()), static_cast<float>(yEdit->value()), static_cast<float>(zEdit->value()));
            property->setValue(componentData, Cube::Any(next));
            notifySceneDirty();
        };
        connect(xEdit, &NumericDragEdit::valueCommitted, this, [commit](double) { commit(); });
        connect(yEdit, &NumericDragEdit::valueCommitted, this, [commit](double) { commit(); });
        connect(zEdit, &NumericDragEdit::valueCommitted, this, [commit](double) { commit(); });

        m_tree->setItemWidget(item, 1, row);
        return;
    }

    if(typeID == Cube::getTypeID<glm::vec4>()) {
        const glm::vec4 vec = value.as<glm::vec4>();
        auto* row = new QWidget(m_tree);
        auto* rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        rowLayout->setSpacing(4);

        auto* xEdit = new NumericDragEdit(false, row);
        auto* yEdit = new NumericDragEdit(false, row);
        auto* zEdit = new NumericDragEdit(false, row);
        auto* wEdit = new NumericDragEdit(false, row);
        xEdit->setValue(vec.x);
        yEdit->setValue(vec.y);
        zEdit->setValue(vec.z);
        wEdit->setValue(vec.w);
        rowLayout->addWidget(xEdit);
        rowLayout->addWidget(yEdit);
        rowLayout->addWidget(zEdit);
        rowLayout->addWidget(wEdit);

        auto commit = [this, property, componentData, xEdit, yEdit, zEdit, wEdit]() {
            glm::vec4 next(static_cast<float>(xEdit->value()), static_cast<float>(yEdit->value()), static_cast<float>(zEdit->value()), static_cast<float>(wEdit->value()));
            property->setValue(componentData, Cube::Any(next));
            notifySceneDirty();
        };
        connect(xEdit, &NumericDragEdit::valueCommitted, this, [commit](double) { commit(); });
        connect(yEdit, &NumericDragEdit::valueCommitted, this, [commit](double) { commit(); });
        connect(zEdit, &NumericDragEdit::valueCommitted, this, [commit](double) { commit(); });
        connect(wEdit, &NumericDragEdit::valueCommitted, this, [commit](double) { commit(); });

        m_tree->setItemWidget(item, 1, row);
        return;
    }

    if(typeID == Cube::getTypeID<Cube::TextureRegion>()) {
        const Cube::TextureRegion region = value.as<Cube::TextureRegion>();
        auto* row = new QWidget(m_tree);
        auto* rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        rowLayout->setSpacing(4);

        auto* minX = new NumericDragEdit(false, row);
        auto* minY = new NumericDragEdit(false, row);
        auto* maxX = new NumericDragEdit(false, row);
        auto* maxY = new NumericDragEdit(false, row);
        minX->setValue(region.uvMin.x);
        minY->setValue(region.uvMin.y);
        maxX->setValue(region.uvMax.x);
        maxY->setValue(region.uvMax.y);
        rowLayout->addWidget(minX);
        rowLayout->addWidget(minY);
        rowLayout->addWidget(maxX);
        rowLayout->addWidget(maxY);

        auto commit = [this, property, componentData, minX, minY, maxX, maxY]() {
            Cube::TextureRegion next;
            next.uvMin = glm::vec2(static_cast<float>(minX->value()), static_cast<float>(minY->value()));
            next.uvMax = glm::vec2(static_cast<float>(maxX->value()), static_cast<float>(maxY->value()));
            property->setValue(componentData, Cube::Any(next));
            notifySceneDirty();
        };
        connect(minX, &NumericDragEdit::valueCommitted, this, [commit](double) { commit(); });
        connect(minY, &NumericDragEdit::valueCommitted, this, [commit](double) { commit(); });
        connect(maxX, &NumericDragEdit::valueCommitted, this, [commit](double) { commit(); });
        connect(maxY, &NumericDragEdit::valueCommitted, this, [commit](double) { commit(); });

        m_tree->setItemWidget(item, 1, row);
        return;
    }

    if(typeID == Cube::getTypeID<Cube::Color>()) {
        const Cube::Color color = value.as<Cube::Color>();
        auto* button = new QPushButton(m_tree);
        const auto refreshButton = [button](const Cube::Color& c) {
            const QColor qColor = QColor::fromRgbF(c.r, c.g, c.b, c.a);
            button->setText(QString("rgba(%1, %2, %3, %4)")
                                .arg(qColor.red())
                                .arg(qColor.green())
                                .arg(qColor.blue())
                                .arg(qColor.alpha()));
            button->setStyleSheet(QString("text-align:left; padding: 0 6px; background-color: rgba(%1,%2,%3,%4);")
                                      .arg(qColor.red())
                                      .arg(qColor.green())
                                      .arg(qColor.blue())
                                      .arg(qColor.alpha()));
        };

        refreshButton(color);
        connect(button, &QPushButton::clicked, this, [this, property, componentData, button, refreshButton] {
            const Cube::Color current = property->getValue(componentData).as<Cube::Color>();
            const QColor initial = QColor::fromRgbF(current.r, current.g, current.b, current.a);
            const QColor picked = QColorDialog::getColor(initial, this, "Pick Color", QColorDialog::ShowAlphaChannel);
            if(!picked.isValid()) {
                return;
            }

            Cube::Color next(picked.redF(), picked.greenF(), picked.blueF(), picked.alphaF());
            property->setValue(componentData, Cube::Any(next));
            notifySceneDirty();
            refreshButton(next);
            button->setText(QString("rgba(%1, %2, %3, %4)")
                                .arg(picked.red())
                                .arg(picked.green())
                                .arg(picked.blue())
                                .arg(picked.alpha()));
        });

        m_tree->setItemWidget(item, 1, button);
        return;
    }

    delete item;
}

QString EntityPropertyView::anyToString(const Cube::Any& value, Cube::TypeID typeID) const {
    if(typeID == Cube::getTypeID<bool>()) {
        return value.as<bool>() ? "true" : "false";
    }
    if(typeID == Cube::getTypeID<int8_t>()) {
        return QString::number(value.as<int8_t>());
    }
    if(typeID == Cube::getTypeID<uint8_t>()) {
        return QString::number(value.as<uint8_t>());
    }
    if(typeID == Cube::getTypeID<int16_t>()) {
        return QString::number(value.as<int16_t>());
    }
    if(typeID == Cube::getTypeID<uint16_t>()) {
        return QString::number(value.as<uint16_t>());
    }
    if(typeID == Cube::getTypeID<int32_t>()) {
        return QString::number(value.as<int32_t>());
    }
    if(typeID == Cube::getTypeID<uint32_t>()) {
        return QString::number(value.as<uint32_t>());
    }
    if(typeID == Cube::getTypeID<int64_t>()) {
        return QString::number(value.as<qint64>());
    }
    if(typeID == Cube::getTypeID<uint64_t>()) {
        return QString::number(value.as<quint64>());
    }
    if(typeID == Cube::getTypeID<float>()) {
        return QString::number(value.as<float>(), 'f', 3);
    }
    if(typeID == Cube::getTypeID<double>()) {
        return QString::number(value.as<double>(), 'f', 3);
    }
    if(typeID == Cube::getTypeID<std::string>()) {
        return QString::fromStdString(value.as<std::string>());
    }

    if(typeID == Cube::getTypeID<glm::vec1>()) {
        const glm::vec1 v = value.as<glm::vec1>();
        return QString("[%1]").arg(v.x, 0, 'f', 3);
    }
    if(typeID == Cube::getTypeID<glm::vec2>()) {
        const glm::vec2 v = value.as<glm::vec2>();
        return QString("[%1, %2]").arg(v.x, 0, 'f', 3).arg(v.y, 0, 'f', 3);
    }
    if(typeID == Cube::getTypeID<glm::vec3>()) {
        const glm::vec3 v = value.as<glm::vec3>();
        return QString("[%1, %2, %3]").arg(v.x, 0, 'f', 3).arg(v.y, 0, 'f', 3).arg(v.z, 0, 'f', 3);
    }
    if(typeID == Cube::getTypeID<glm::vec4>()) {
        const glm::vec4 v = value.as<glm::vec4>();
        return QString("[%1, %2, %3, %4]").arg(v.x, 0, 'f', 3).arg(v.y, 0, 'f', 3).arg(v.z, 0, 'f', 3).arg(v.w, 0, 'f', 3);
    }
    if(typeID == Cube::getTypeID<Cube::TextureRegion>()) {
        const Cube::TextureRegion r = value.as<Cube::TextureRegion>();
        return QString("uvMin[%1, %2], uvMax[%3, %4]")
            .arg(r.uvMin.x, 0, 'f', 3)
            .arg(r.uvMin.y, 0, 'f', 3)
            .arg(r.uvMax.x, 0, 'f', 3)
            .arg(r.uvMax.y, 0, 'f', 3);
    }
    if(typeID == Cube::getTypeID<Cube::Color>()) {
        const Cube::Color c = value.as<Cube::Color>();
        return QString("rgba(%1, %2, %3, %4)")
            .arg(c.r, 0, 'f', 3)
            .arg(c.g, 0, 'f', 3)
            .arg(c.b, 0, 'f', 3)
            .arg(c.a, 0, 'f', 3);
    }

    return QStringLiteral("<Unsupported>");
}
