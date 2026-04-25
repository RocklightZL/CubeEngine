#include "EntityPropertyView.h"

#include "Cube/Reflection/Any.h"
#include "Cube/Reflection/Class.h"
#include "Cube/Reflection/ClassRegistry.h"
#include "Cube/Reflection/Serializer.h"
#include "Cube/Scene/Component.h"
#include "Cube/Scene/Entity.h"
#include "Cube/Scene/Scene.h"
#include "Cube/Scene/Transform.h"

#include <QJsonDocument>
#include <QJsonValue>
#include <QTreeWidget>
#include <QVBoxLayout>

namespace {
constexpr int kMaxReflectionDepth = 3;
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

    const Cube::Transform& transform = m_entity->getTransform();

    const glm::vec2 position = transform.getPosition();
    auto* posItem = new QTreeWidgetItem(transformItem);
    posItem->setText(0, "Position");
    posItem->setText(1, QString("(%1, %2)").arg(position.x, 0, 'f', 3).arg(position.y, 0, 'f', 3));

    auto* rotItem = new QTreeWidgetItem(transformItem);
    rotItem->setText(0, "Rotation");
    rotItem->setText(1, QString::number(transform.getRotation(), 'f', 3));

    const glm::vec2 scale = transform.getScale();
    auto* scaleItem = new QTreeWidgetItem(transformItem);
    scaleItem->setText(0, "Scale");
    scaleItem->setText(1, QString("(%1, %2)").arg(scale.x, 0, 'f', 3).arg(scale.y, 0, 'f', 3));
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
        appendPropertyRecursive(componentItem, property, value, 0);
    }
}

void EntityPropertyView::appendPropertyRecursive(QTreeWidgetItem* parent,
                                                 const Cube::Property* property,
                                                 const Cube::Any& value,
                                                 int depth) {
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
            appendPropertyRecursive(item, nestedProp, nestedValue, depth + 1);
        }
        return;
    }

    item->setText(1, anyToString(value, typeID));
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
