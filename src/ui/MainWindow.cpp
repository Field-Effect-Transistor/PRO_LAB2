//  MainWindow.cpp

#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    left = new MenuWidget();
    right = new EnterWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(left);
    mainLayout->addWidget(right);

    setLayout(mainLayout);
}

MainWindow::~MainWindow() {}