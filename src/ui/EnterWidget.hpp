//  EnterWidget.hpp

#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>

class EnterWidget : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout*    mainLayout;
    QTableWidget*   table;

    QPushButton*    confirmButton;
    QPushButton*    randomButton;

public:
    EnterWidget(QWidget *parent = nullptr);
    ~EnterWidget();
};