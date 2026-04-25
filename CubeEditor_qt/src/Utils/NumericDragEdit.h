#pragma once

#include <QtWidgets/QWidget>

class QMouseEvent;
class QStackedLayout;
class QLabel;
class QLineEdit;

class NumericDragEdit final : public QWidget {
    Q_OBJECT

public:
    explicit NumericDragEdit(bool integerMode, QWidget* parent = nullptr);

    void setValue(double value);
    double value() const;

signals:
    void valueCommitted(double value);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    void beginTextEdit();
    void finishTextEdit();
    void updateDisplayText();

private:
    bool m_integerMode = false;
    bool m_dragging = false;
    bool m_editingText = false;
    int m_dragStartX = 0;
    double m_dragStartValue = 0.0;
    double m_value = 0.0;
    double m_dragStep = 0.1;

    QStackedLayout* m_stack = nullptr;
    QLabel* m_label = nullptr;
    QLineEdit* m_editor = nullptr;
};
