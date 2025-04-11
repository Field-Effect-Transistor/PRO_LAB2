// NotificationWidget.hpp
#pragma once

#include <QMessageBox>
#include <QString>

class NotificationWidget : public QMessageBox {
public:
    explicit NotificationWidget(const QString& text, QWidget* parent = nullptr);
};
