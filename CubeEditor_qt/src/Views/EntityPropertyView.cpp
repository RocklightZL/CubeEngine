#include "EntityPropertyView.h"

#include "../Utils/NumericDragEdit.h"

#include "Cube/Reflection/Any.h"
#include "Cube/Reflection/Class.h"
#include "Cube/Reflection/ClassRegistry.h"
#include "Cube/Reflection/Serializer.h"
#include "Cube/Scene/Component.h"
#include "Cube/Scene/Entity.h"
#include "Cube/Scene/Scene.h"
#include "Cube/Scene/Transform.h"

#include <QCheckBox>
#include <QJsonDocument>
#include <QJsonValue>
#include <QLineEdit>
#include <QTreeWidget>
#include <QVBoxLayout>

namespace {
constexpr int kMaxReflectionDepth = 3;

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
    layout->setSpacing(0);

    m_tree = new QTreeWidget(this);
    m_tree->setObjectName("entityPropertyTree");
    m_tree->setColumnCount(2);
    m_tree->setHeaderLabels(QStringList{"Property", "Value"});
    m_tree->setRootIsDecorated(true);
    m_tree->setAlternatingRowColors(false);
    m_tree->setStyleSheet(R"(
        QTreeWidget#entityPropertyTree {
            border: none;
            background: transparent;
            outline: none;
            color: #f3f3f3;
        }
        QTreeWidget#entityPropertyTree::item {
            height: 24px;
        }
        QTreeWidget#entityPropertyTree::item:hover {
            background: #313136;
        }
        QTreeWidget#entityPropertyTree::item:selected {
            background: #094771;
            color: #ffffff;
        }
        QHeaderView::section {
            background: #2d2d30;
            color: #d6d6d6;
            border: 1px solid #3f3f46;
            padding-left: 6px;
            height: 22px;
        }
    )");

    layout->addWidget(m_tree);
    refresh();
}

void EntityPropertyView::setSelection(Cube::Scene* scene, Cube::Entity* entity) {
    m_scene = scene;
    m_entity = entity;
    refresh();
}

void EntityPropertyView::refresh() {
    m_tree->clear();

    if(!m_scene || !m_entity) {
        auto* hint = new QTreeWidgetItem(m_tree);
        hint->setText(0, "No entity selected.");
        hint->setFirstColumnSpanned(true);
        return;
    }

    auto* entityRoot = new QTreeWidgetItem(m_tree);
    entityRoot->setText(0, "Entity");
    entityRoot->setText(1, QString::fromStdString(m_entity->getName()));
    entityRoot->setExpanded(true);

    appendTransform(entityRoot);

    auto* componentsRoot = new QTreeWidgetItem(entityRoot);
    componentsRoot->setText(0, "Components");
    componentsRoot->setExpanded(true);

    const auto& components = m_entity->getComponents();
    for(const auto& component : components) {
        if(!component) {
            continue;
        }

        Cube::Class* classInfo = Cube::ClassRegistry::get().getClass(component->getType());
        const QString componentName = classInfo ? QString::fromStdString(classInfo->getName()) : QStringLiteral("UnknownComponent");
        appendComponent(componentsRoot, componentName, component.get(), classInfo);
    }

    m_tree->expandAll();
    m_tree->resizeColumnToContents(0);
}

void EntityPropertyView::appendTransform(QTreeWidgetItem* root) {
    auto* transformItem = new QTreeWidgetItem(root);
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

    const glm::vec2 position = transform.getPosition();
    auto* posXItem = new QTreeWidgetItem(transformItem);
    posXItem->setText(0, "Position.x");
    createFloatEditor(posXItem, position.x, [this](double v) {
        auto p = m_entity->getTransform().getPosition();
        p.x = static_cast<float>(v);
        m_entity->getTransform().setPosition(p);
    });

    auto* posYItem = new QTreeWidgetItem(transformItem);
    posYItem->setText(0, "Position.y");
    createFloatEditor(posYItem, position.y, [this](double v) {
        auto p = m_entity->getTransform().getPosition();
        p.y = static_cast<float>(v);
        m_entity->getTransform().setPosition(p);
    });

    auto* rotItem = new QTreeWidgetItem(transformItem);
    rotItem->setText(0, "Rotation");
    createFloatEditor(rotItem, transform.getRotation(), [this](double v) {
        m_entity->getTransform().setRotation(static_cast<float>(v));
    });

    const glm::vec2 scale = transform.getScale();
    auto* scaleXItem = new QTreeWidgetItem(transformItem);
    scaleXItem->setText(0, "Scale.x");
    createFloatEditor(scaleXItem, scale.x, [this](double v) {
        auto s = m_entity->getTransform().getScale();
        s.x = static_cast<float>(v);
        m_entity->getTransform().setScale(s);
    });

    auto* scaleYItem = new QTreeWidgetItem(transformItem);
    scaleYItem->setText(0, "Scale.y");
    createFloatEditor(scaleYItem, scale.y, [this](double v) {
        auto s = m_entity->getTransform().getScale();
        s.y = static_cast<float>(v);
        m_entity->getTransform().setScale(s);
    });
}

void EntityPropertyView::appendComponent(QTreeWidgetItem* root,
                                         const QString& componentName,
                                         void* componentData,
                                         const Cube::Class* classInfo) {
    auto* componentItem = new QTreeWidgetItem(root);
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
        appendPropertyRecursive(componentItem, componentData, std::vector<const Cube::Property*>{property}, value, 0);
    }
}

void EntityPropertyView::appendPropertyRecursive(QTreeWidgetItem* parent,
                                                 void* rootObjectData,
                                                 const std::vector<const Cube::Property*>& propertyPath,
                                                 const Cube::Any& value,
                                                 int depth) {
    if(propertyPath.empty()) {
        return;
    }

    const Cube::Property* property = propertyPath.back();

    auto* item = new QTreeWidgetItem(parent);
    item->setText(0, QString::fromStdString(property->getName()));

    if(depth >= kMaxReflectionDepth) {
        item->setText(1, "<Max depth reached>");
        return;
    }

    const Cube::TypeID typeID = property->getTypeID();
    Cube::Class* nestedClass = Cube::ClassRegistry::get().getClass(Cube::removeAllPtr(typeID));

    if(!Cube::isPtr(typeID) && nestedClass && !nestedClass->getAllProperties().empty()) {
        item->setText(1, QString::fromStdString(nestedClass->getName()));
        for(const Cube::Property* nestedProp : nestedClass->getAllProperties()) {
            if(!nestedProp) {
                continue;
            }
            Cube::Any nestedValue = nestedProp->getValue(value.getData());
            auto childPath = propertyPath;
            childPath.push_back(nestedProp);
            appendPropertyRecursive(item, rootObjectData, childPath, nestedValue, depth + 1);
        }
        return;
    }

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

        connect(editor, &NumericDragEdit::valueCommitted, this, [this, rootObjectData, propertyPath, typeID](double v) {
            if(typeID == Cube::getTypeID<float>()) {
                setPropertyByPath(rootObjectData, propertyPath, Cube::Any(static_cast<float>(v)));
            } else if(typeID == Cube::getTypeID<double>()) {
                setPropertyByPath(rootObjectData, propertyPath, Cube::Any(static_cast<double>(v)));
            } else if(typeID == Cube::getTypeID<int8_t>()) {
                setPropertyByPath(rootObjectData, propertyPath, Cube::Any(static_cast<int8_t>(v)));
            } else if(typeID == Cube::getTypeID<uint8_t>()) {
                setPropertyByPath(rootObjectData, propertyPath, Cube::Any(static_cast<uint8_t>(v)));
            } else if(typeID == Cube::getTypeID<int16_t>()) {
                setPropertyByPath(rootObjectData, propertyPath, Cube::Any(static_cast<int16_t>(v)));
            } else if(typeID == Cube::getTypeID<uint16_t>()) {
                setPropertyByPath(rootObjectData, propertyPath, Cube::Any(static_cast<uint16_t>(v)));
            } else if(typeID == Cube::getTypeID<int32_t>()) {
                setPropertyByPath(rootObjectData, propertyPath, Cube::Any(static_cast<int32_t>(v)));
            } else if(typeID == Cube::getTypeID<uint32_t>()) {
                setPropertyByPath(rootObjectData, propertyPath, Cube::Any(static_cast<uint32_t>(v)));
            } else if(typeID == Cube::getTypeID<int64_t>()) {
                setPropertyByPath(rootObjectData, propertyPath, Cube::Any(static_cast<int64_t>(v)));
            } else if(typeID == Cube::getTypeID<uint64_t>()) {
                setPropertyByPath(rootObjectData, propertyPath, Cube::Any(static_cast<uint64_t>(v)));
            }
        });

        m_tree->setItemWidget(item, 1, editor);
        return;
    }

    if(typeID == Cube::getTypeID<bool>()) {
        auto* check = new QCheckBox(m_tree);
        check->setChecked(value.as<bool>());
        connect(check, &QCheckBox::toggled, this, [this, rootObjectData, propertyPath](bool checked) {
            setPropertyByPath(rootObjectData, propertyPath, Cube::Any(checked));
        });
        m_tree->setItemWidget(item, 1, check);
        return;
    }

    if(typeID == Cube::getTypeID<std::string>()) {
        auto* edit = new QLineEdit(QString::fromStdString(value.as<std::string>()), m_tree);
        edit->setFrame(false);
        connect(edit, &QLineEdit::editingFinished, this, [this, rootObjectData, propertyPath, edit] {
            setPropertyByPath(rootObjectData, propertyPath, Cube::Any(edit->text().toStdString()));
        });
        m_tree->setItemWidget(item, 1, edit);
        return;
    }

    item->setText(1, anyToString(value, typeID));
}

bool EntityPropertyView::setPropertyByPath(void* rootObjectData,
                                           const std::vector<const Cube::Property*>& propertyPath,
                                           Cube::Any&& newValue) {
    if(!rootObjectData || propertyPath.empty()) {
        return false;
    }

    return setPropertyByPathRecursive(rootObjectData, propertyPath, 0, std::move(newValue));
}

bool EntityPropertyView::setPropertyByPathRecursive(void* ownerObjectData,
                                                    const std::vector<const Cube::Property*>& propertyPath,
                                                    int pathIndex,
                                                    Cube::Any&& newValue) {
    if(pathIndex < 0 || pathIndex >= static_cast<int>(propertyPath.size())) {
        return false;
    }

    const Cube::Property* property = propertyPath[pathIndex];
    if(!property) {
        return false;
    }

    if(pathIndex == static_cast<int>(propertyPath.size()) - 1) {
        property->setValue(ownerObjectData, std::move(newValue));
        return true;
    }

    Cube::Any child = property->getValue(ownerObjectData);
    const bool ok = setPropertyByPathRecursive(child.getData(), propertyPath, pathIndex + 1, std::move(newValue));
    if(ok) {
        property->setValue(ownerObjectData, std::move(child));
    }
    return ok;
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
        return QString::number(value.as<float>(), 'f', 6);
    }
    if(typeID == Cube::getTypeID<double>()) {
        return QString::number(value.as<double>(), 'f', 6);
    }
    if(typeID == Cube::getTypeID<std::string>()) {
        return QString::fromStdString(value.as<std::string>());
    }

    if(Cube::Serializer::get().isRegistered(typeID)) {
        nlohmann::json jsonValue = Cube::Serializer::get().serialize(typeID, value);
        return QString::fromStdString(jsonValue.dump());
    }

    Cube::Class* classInfo = Cube::ClassRegistry::get().getClass(Cube::removeAllPtr(typeID));
    if(classInfo) {
        return QString::fromStdString(classInfo->getName());
    }

    return QStringLiteral("<Unsupported>");
}
