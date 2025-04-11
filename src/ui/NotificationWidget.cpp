//  NotificationWidget.cpp

#include "NotificationWidget.hpp"

NotificationWidget::NotificationWidget(const QString& text, QWidget* parent)
: QMessageBox(parent) {
    setIcon(QMessageBox::Information);
    setText(text);
    setWindowTitle("Notification");
}