//  EnterWidget.cpp

#include "EnterWidget.hpp"

EnterWidget::EnterWidget(QWidget *parent) : QWidget(parent) {
    mainLayout = new QVBoxLayout;
    randomButton = new QPushButton("Random");
    confirmButton = new QPushButton("Confirm");

    table = new QTableWidget;
    mainLayout->addStretch();
    mainLayout->addWidget(table);
    mainLayout->addWidget(randomButton);
    mainLayout->addStretch();
    mainLayout->addWidget(confirmButton);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

EnterWidget::~EnterWidget() {}