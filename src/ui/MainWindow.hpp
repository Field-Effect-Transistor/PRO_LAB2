//  MainWindow.hpp

#pragma once

#include <QWidget>
#include <QHBoxLayout>

#include "MenuWidget.hpp"
#include "EnterWidget.hpp"

class MainWindow: public QWidget {
    Q_OBJECT
private:
    QWidget *left;
    QWidget *right;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
