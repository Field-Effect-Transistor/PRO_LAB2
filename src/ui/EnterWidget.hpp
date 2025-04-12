//  EnterWidget.hpp

#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include "MenuWidget.hpp"
#include "../logic/Math.hpp"

class EnterWidget : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout*    mainLayout;
    QTableWidget*   table;


public:
    EnterWidget(QWidget *parent = nullptr);
    ~EnterWidget();

    QPushButton*    confirmButton;
    QPushButton*    randomButton;

    static double* vectorB1;
    static double* vectorC1;
    static double** matrixA;
    static double** matrixA1;
    static double** matrixA2;
    static double** matrixB2;

    int n = 4;

public slots:
    void initTable(int n, QRadioButton* radio);
    void randomize();
    void confirm(QRadioButton* radio);
};