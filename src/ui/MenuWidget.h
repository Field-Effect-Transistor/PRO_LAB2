// MenuWidget.h

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

class MenuWidget : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout *mainLayout;

    QGroupBox   *statementsBox;
    QVBoxLayout *statementsLayout;

    QGroupBox   *setNBox;
    QHBoxLayout *setNLayout;
    QSpinBox    *setNSpinBox;
    QPushButton *confirmNButton;

    QGroupBox   *setMatrixBox;
    QHBoxLayout *setMatrixLayout;
    QButtonGroup    *setMatrixButtonGroup;

    QPushButton *startButton;
    
public:
    MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget();
};