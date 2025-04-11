// MenuWidget.hpp

#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QSpinBox>

#include "../logic/MPIHandler.hpp"
#include "EnterWidget.hpp"

class MenuWidget : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout *mainLayout;

    QGroupBox   *statementsBox;
    QVBoxLayout *statementsLayout;

    QGroupBox   *setNBox;
    QHBoxLayout *setNLayout;

    QGroupBox   *setMatrixBox;
    QHBoxLayout *setMatrixLayout;

    
public:

    QPushButton *startButton;
    QPushButton *confirmNButton;
    QSpinBox    *setNSpinBox;
    QButtonGroup    *setMatrixButtonGroup;

static QRadioButton
    *matrixA,
    *matrixA1,
    *matrixA2,
    *matrixB2,
    *vectorB1,
    *vectorC1;

    MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget();

public slots:
    void start(int n);

};