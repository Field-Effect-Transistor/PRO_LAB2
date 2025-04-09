//  MainWindow.cpp

#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    left = new MenuWidget();
    right = new EnterWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(left);
    left->setMaximumHeight(490);
    left->setMinimumWidth(300);
    left->setMaximumWidth(320);
    mainLayout->addWidget(right);

    setLayout(mainLayout);

    connect(
        left->confirmNButton,
        &QPushButton::clicked,
        this,
        [this]() {
            right->initTable(left->setNSpinBox->value(), 
                            (QRadioButton*)left->setMatrixButtonGroup->checkedButton());
        }
    );

    connect(
        left->setMatrixButtonGroup,
        QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
        this,
        [this]() {
            right->initTable(left->setNSpinBox->value(), 
                            (QRadioButton*)left->setMatrixButtonGroup->checkedButton());
        }
    );
    
}

MainWindow::~MainWindow() {}