#pragma once

#include <QWidget>

#include "Cube/Reflection/Type.h"

#include <vector>

namespace Cube {
    class Entity;
    class Scene;
    class Class;
    class Property;
    class Any;
}

class QTreeWidget;
class QTreeWidgetItem;

class EntityPropertyView final : public QWidget {
public:
    explicit EntityPropertyView(QWidget* parent = nullptr);

    void setSelection(Cube::Scene* scene, Cube::Entity* entity);

private:
    void refresh();
    void appendTransform(QTreeWidgetItem* root);
    void appendComponent(QTreeWidgetItem* root, const QString& componentName, void* componentData, const Cube::Class* classInfo);
    void appendPropertyRecursive(QTreeWidgetItem* parent,
                                 void* rootObjectData,
                                 const std::vector<const Cube::Property*>& propertyPath,
                                 const Cube::Any& value,
                                 int depth);
    bool setPropertyByPath(void* rootObjectData, const std::vector<const Cube::Property*>& propertyPath, Cube::Any&& newValue);
    bool setPropertyByPathRecursive(void* ownerObjectData,
                                    const std::vector<const Cube::Property*>& propertyPath,
                                    int pathIndex,
                                    Cube::Any&& newValue);
    QString anyToString(const Cube::Any& value, Cube::TypeID typeID) const;

private:
    Cube::Scene* m_scene = nullptr;
    Cube::Entity* m_entity = nullptr;
    QTreeWidget* m_tree = nullptr;
};
