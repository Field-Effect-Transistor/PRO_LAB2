//  EnterWidget.hpp

#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "MenuWidget.hpp"

using namespace boost::numeric::ublas;

class EnterWidget : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout*    mainLayout;
    QTableWidget*   table;

    int n = 4;

public:
    EnterWidget(QWidget *parent = nullptr);
    ~EnterWidget();

    QPushButton*    confirmButton;
    QPushButton*    randomButton;

    static vector<double> vectorB1;
    static vector<double> vectorC1;
    static matrix<double> matrixA;
    static matrix<double> matrixA1;
    static matrix<double> matrixA2;
    static matrix<double> matrixB2;

public slots:
    void initTable(int n, QRadioButton* radio);
    void randomize();
    void confirm(QRadioButton* radio);
};