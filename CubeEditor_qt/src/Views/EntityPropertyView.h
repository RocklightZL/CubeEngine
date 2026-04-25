#pragma once

#include <QWidget>

#include "Cube/Reflection/Type.h"

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
    void appendPropertyRecursive(QTreeWidgetItem* parent, const Cube::Property* property, const Cube::Any& value, int depth);
    QString anyToString(const Cube::Any& value, Cube::TypeID typeID) const;

private:
    Cube::Scene* m_scene = nullptr;
    Cube::Entity* m_entity = nullptr;
    QTreeWidget* m_tree = nullptr;
};
