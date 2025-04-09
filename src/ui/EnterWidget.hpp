//  EnterWidget.hpp

#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include "MenuWidget.hpp"

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

    static int* vectorB1;
    static int* vectorC1;
    static int** matrixA;
    static int** matrixA1;
    static int** matrixA2;
    static int** matrixB2;

public slots:
    void initTable(int n, QRadioButton* radio);
    //void randomize();
    //void confirm();
};