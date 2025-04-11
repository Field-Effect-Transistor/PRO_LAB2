//  MainWindow.hpp

#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QCloseEvent>

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

    static bool flag;

    void closeEvent(QCloseEvent* event) override;
};
