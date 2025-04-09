//  MainWindow.cpp

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    left = new MenuWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(left);
    setLayout(mainLayout);
    //right = new QWidget();
}

MainWindow::~MainWindow() {}