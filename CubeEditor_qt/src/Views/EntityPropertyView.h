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
class QLabel;

class EntityPropertyView final : public QWidget {
public:
    explicit EntityPropertyView(QWidget* parent = nullptr);

    void setSelection(Cube::Scene* scene, Cube::Entity* entity);

private:
    void refresh();
    void appendTransform();
    void appendComponent(const QString& componentName, void* componentData, const Cube::Class* classInfo);
    void appendPropertyFlat(QTreeWidgetItem* parent,
                            void* componentData,
                            const Cube::Property* property,
                            const Cube::Any& value);
    QString anyToString(const Cube::Any& value, Cube::TypeID typeID) const;

private:
    Cube::Scene* m_scene = nullptr;
    Cube::Entity* m_entity = nullptr;
    QLabel* m_entityNameLabel = nullptr;
    QTreeWidget* m_tree = nullptr;
};
