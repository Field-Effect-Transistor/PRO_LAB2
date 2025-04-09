//  MainWindow.hpp

#pragma once

#include <QWidget>
#include <QHBoxLayout>

#include "MenuWidget.hpp"
#include "EnterWidget.hpp"

class MainWindow: public QWidget {
    Q_OBJECT
private:
    MenuWidget *left;
    EnterWidget *right;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
