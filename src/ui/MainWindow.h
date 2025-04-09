//  MainWindow.h

#pragma once

#include <QWidget>
#include <QHBoxLayout>

#include "MenuWidget.h"

class MainWindow: public QWidget {
    Q_OBJECT
private:
    QWidget *left;
    //QWidget *right;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
