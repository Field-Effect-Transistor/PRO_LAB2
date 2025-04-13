//  MainWindow.cpp

#include "MainWindow.hpp"

bool MainWindow::flag = false;

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
    
    connect(
        right->confirmButton,
        &QPushButton::clicked,
        this,
        [this]() {
            right->confirm((QRadioButton*)left->setMatrixButtonGroup->checkedButton());
        }
    );

    connect(
        left->startButton,
        &QPushButton::clicked,
        this,
        [this]() {
            left->start(right->n);
        }
    );

}

MainWindow::~MainWindow() {}

void MainWindow::closeEvent(QCloseEvent* event)  {
    // Виконуємо необхідну функцію перед закриттям
    //  MPI_Abort(MPI_COMM_WORLD, 1);
#ifdef MPI_ON
    bool isExit = true;
    for (int i = 0; i < MPIHandler::getSize(); i++) {
        MPI_Send(&isExit, 1, MPI_CXX_BOOL, i, 0, MPI_COMM_WORLD);
    }
#endif
    event->accept(); // Приймаємо подію закриття
}