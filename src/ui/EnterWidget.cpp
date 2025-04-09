//  EnterWidget.cpp

#include "EnterWidget.hpp"

int* EnterWidget::vectorB1 = nullptr;
int* EnterWidget::vectorC1 = nullptr;
int** EnterWidget::matrixA = nullptr;
int** EnterWidget::matrixA1 = nullptr;
int** EnterWidget::matrixA2 = nullptr;
int** EnterWidget::matrixB2 = nullptr;

EnterWidget::EnterWidget(QWidget *parent) : QWidget(parent) {
    mainLayout = new QVBoxLayout;
    randomButton = new QPushButton("Random");
    confirmButton = new QPushButton("Confirm");

    table = new QTableWidget(4,4);
    table->resizeColumnsToContents();
    mainLayout->addStretch();
    mainLayout->addWidget(table);
    mainLayout->addWidget(randomButton);
    mainLayout->addStretch();
    mainLayout->addWidget(confirmButton);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

EnterWidget::~EnterWidget() {}

void EnterWidget::initTable(int n, QRadioButton* radio) {
    table->setRowCount(n);
    if(radio == MenuWidget::vectorB1 or radio == MenuWidget::vectorC1) {
        table->setColumnCount(1);
    } else {
        table->setColumnCount(n);
    }
    table->resizeColumnsToContents();
}