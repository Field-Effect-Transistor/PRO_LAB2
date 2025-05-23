//  EnterWidget.cpp

#include "EnterWidget.hpp"

vector<double> EnterWidget::vectorB1(10, 0);
vector<double> EnterWidget::vectorC1(10, 0);
matrix<double> EnterWidget::matrixA(10, 10, 0);
matrix<double> EnterWidget::matrixA1(10, 10, 0);
matrix<double> EnterWidget::matrixA2(10, 10, 0);
matrix<double> EnterWidget::matrixB2(10, 10, 0);

EnterWidget::EnterWidget(QWidget *parent) : QWidget(parent) {
    mainLayout = new QVBoxLayout;
    randomButton = new QPushButton("Random");
    confirmButton = new QPushButton("Confirm");

    table = new QTableWidget(n,n);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            table->setItem(i, j, new QTableWidgetItem("0"));
        }
    }
    table->resizeColumnsToContents();
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->setMinimumWidth(375);
    mainLayout->addWidget(table);
    mainLayout->addWidget(randomButton);
    mainLayout->addWidget(confirmButton);
    setLayout(mainLayout);

    connect(
        randomButton,
        &QPushButton::clicked,
        this,
        &EnterWidget::randomize
    );
}

EnterWidget::~EnterWidget() {}

void EnterWidget::initTable(int n, QRadioButton* radio) {
    this->n = n;
    table->setRowCount(n);
    if(radio == MenuWidget::vectorB1 or radio == MenuWidget::vectorC1) {
        table->setColumnCount(1);
        vector<double>& temp = (radio == MenuWidget::vectorB1) ? EnterWidget::vectorB1 : EnterWidget::vectorC1;
        for(int i = 0; i < n; i++) {
            table->setItem(i, 0, new QTableWidgetItem(QString::number(temp[i])));
        }
    } else {
        table->setColumnCount(n);
        matrix<double>& temp = (radio == MenuWidget::matrixA) ? EnterWidget::matrixA :
            (radio == MenuWidget::matrixA1) ? EnterWidget::matrixA1 :
            (radio == MenuWidget::matrixA2) ? EnterWidget::matrixA2 : EnterWidget::matrixB2;
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                table->setItem(i, j, new QTableWidgetItem(QString::number(temp(i, j))));
            }
        }

    }
    table->resizeColumnsToContents();
}

void    EnterWidget::randomize() {
    for(int i = 0; i < table->rowCount(); i++) {
        for(int j = 0; j < table->columnCount(); j++) {
            table->setItem(i, j, new QTableWidgetItem(QString::number(rand() % 100)));
        }
    }

}

void    EnterWidget::confirm(QRadioButton* radio) {
    if (radio == MenuWidget::vectorB1 or radio == MenuWidget::vectorC1) {
        vector<double>& temp = (radio == MenuWidget::vectorB1) ? EnterWidget::vectorB1 : EnterWidget::vectorC1;
        for(int i = 0; i < n; i++) {
            temp[i] = table->item(i, 0)->text().toDouble();
        }
    } else {
        matrix<double>& temp = (radio == MenuWidget::matrixA) ? EnterWidget::matrixA :
            (radio == MenuWidget::matrixA1) ? EnterWidget::matrixA1 :
            (radio == MenuWidget::matrixA2) ? EnterWidget::matrixA2 : EnterWidget::matrixB2;
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                temp(i, j) = table->item(i, j)->text().toDouble();
            }
        }
    }

}
