#include "NumericDragEdit.h"

#include <QtCore/Qt>
#include <QtGui/QDoubleValidator>
#include <QtGui/QIntValidator>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QStackedLayout>

#include <cmath>

NumericDragEdit::NumericDragEdit(bool integerMode, QWidget* parent)
    : QWidget(parent)
    , m_integerMode(integerMode) {
    setObjectName("numericDragEdit");
    m_dragStep = m_integerMode ? 1.0 : 0.05;
    setFixedHeight(22);

    auto* outer = new QHBoxLayout(this);
    outer->setContentsMargins(0, 0, 0, 0);

    m_stack = new QStackedLayout();
    m_stack->setContentsMargins(0, 0, 0, 0);
    outer->addLayout(m_stack);

    m_label = new QLabel(this);
    m_label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_label->setContentsMargins(4, 0, 4, 0);
    m_stack->addWidget(m_label);

    m_editor = new QLineEdit(this);
    m_editor->setFrame(false);
    m_editor->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    if(m_integerMode) {
        m_editor->setValidator(new QIntValidator(m_editor));
    } else {
        auto* validator = new QDoubleValidator(m_editor);
        validator->setDecimals(6);
        m_editor->setValidator(validator);
    }
    m_stack->addWidget(m_editor);

    connect(m_editor, &QLineEdit::editingFinished, this, &NumericDragEdit::finishTextEdit);

    m_stack->setCurrentWidget(m_label);
    updateDisplayText();
}

void NumericDragEdit::setValue(double value) {
    if(m_integerMode) {
        value = std::round(value);
    }
    m_value = value;
    updateDisplayText();
}

double NumericDragEdit::value() const {
    return m_value;
}

void NumericDragEdit::mousePressEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton && !m_editingText) {
        m_dragging = false;
        m_dragStartX = static_cast<int>(event->position().x());
        m_dragStartValue = m_value;
        event->accept();
        return;
    }
    QWidget::mousePressEvent(event);
}

void NumericDragEdit::mouseMoveEvent(QMouseEvent* event) {
    if((event->buttons() & Qt::LeftButton) && !m_editingText) {
        const int dx = static_cast<int>(event->position().x()) - m_dragStartX;
        if(std::abs(dx) >= 1) {
            m_dragging = true;
            double next = m_dragStartValue + static_cast<double>(dx) * m_dragStep;
            if(m_integerMode) {
                next = std::round(next);
            }
            if(next != m_value) {
                m_value = next;
                updateDisplayText();
                emit valueCommitted(m_value);
            }
        }
        event->accept();
        return;
    }
    QWidget::mouseMoveEvent(event);
}

void NumericDragEdit::mouseReleaseEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        event->accept();
        return;
    }
    QWidget::mouseReleaseEvent(event);
}

void NumericDragEdit::mouseDoubleClickEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        beginTextEdit();
        event->accept();
        return;
    }
    QWidget::mouseDoubleClickEvent(event);
}

void NumericDragEdit::beginTextEdit() {
    m_editingText = true;
    m_editor->setText(m_integerMode ? QString::number(static_cast<qint64>(std::llround(m_value)))
                                    : QString::number(m_value, 'g', 12));
    m_stack->setCurrentWidget(m_editor);
    m_editor->setFocus();
    m_editor->selectAll();
}

void NumericDragEdit::finishTextEdit() {
    if(!m_editingText) {
        return;
    }

    bool ok = false;
    const double parsed = m_editor->text().toDouble(&ok);
    if(ok) {
        setValue(parsed);
        emit valueCommitted(m_value);
    }

    m_editingText = false;
    m_stack->setCurrentWidget(m_label);
}

void NumericDragEdit::updateDisplayText() {
    if(m_integerMode) {
        m_label->setText(QString::number(static_cast<qint64>(std::llround(m_value))));
    } else {
        m_label->setText(QString::number(m_value, 'f', 6));
    }
}
