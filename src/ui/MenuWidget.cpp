//  MenuWidget.cpp

#include "MenuWidget.hpp"

QRadioButton* MenuWidget::matrixA = nullptr;
QRadioButton* MenuWidget::matrixA1 = nullptr;
QRadioButton* MenuWidget::matrixA2 = nullptr;
QRadioButton* MenuWidget::matrixB2 = nullptr;
QRadioButton* MenuWidget::vectorB1 = nullptr;
QRadioButton* MenuWidget::vectorC1 = nullptr;

MenuWidget::MenuWidget(QWidget *parent) : QWidget(parent) {
    mainLayout = new QVBoxLayout;

    statementsBox = new QGroupBox("Statements");
    statementsLayout = new QVBoxLayout;
    statementsBox->setLayout(statementsLayout);

    QWidget *XWidget = new QWidget;
    QBoxLayout *XLayout = new QHBoxLayout;
    XWidget->setLayout(XLayout);
    XLayout->addStretch();
    XLayout->addWidget(new QLabel("X = (y₁`Y₃ * y₁ + y₂`)*(Y₃*y₂ + y₁ + y₁y₂`Y₃²y₂)"));
    XLayout->addStretch();
    statementsLayout->addWidget(XWidget);

    statementsLayout->addWidget(new QLabel("bi = 26i³"));
    statementsLayout->addWidget(new QLabel("y₁ = Ab"));
    statementsLayout->addWidget(new QLabel("y₂ = A₁(26b₁ - c₁)"));
    statementsLayout->addWidget(new QLabel("Y₃ = A₂(B₂ - 26C₂)"));
    statementsLayout->addWidget(new QLabel("Cij = 1 / (i² + j)"));
    //statementsLayout->addStretch();
    
    setNBox = new QGroupBox("Set N");
    setNLayout = new QHBoxLayout;
    setNSpinBox = new QSpinBox;
    confirmNButton = new QPushButton("Confirm");
    setNSpinBox->setMinimum(1);
    setNSpinBox->setValue(4);
    setNSpinBox->setMaximum(10);
    setNLayout->addWidget(setNSpinBox);
    setNLayout->addStretch();
    setNLayout->addWidget(confirmNButton);
    setNBox->setLayout(setNLayout);

    setMatrixBox = new QGroupBox("Set Matrix");
    setMatrixLayout = new QHBoxLayout;
    QWidget* left = new QWidget, *right = new QWidget;
    QVBoxLayout* leftLayout = new QVBoxLayout, *rightLayout = new QVBoxLayout;
    matrixA = new QRadioButton("Matrix A"),
    matrixA1 = new QRadioButton("Matrix A1"),
    matrixA2 = new QRadioButton("Matrix A2"),
    matrixB2 = new QRadioButton("Matrix B2"),
    vectorB1 = new QRadioButton("Vector b1"),
    vectorC1 = new QRadioButton("Vector c1");
    leftLayout->addWidget(matrixA);
    leftLayout->addWidget(matrixA1);
    leftLayout->addWidget(matrixA2);
    rightLayout->addWidget(matrixB2);
    rightLayout->addWidget(vectorB1);
    rightLayout->addWidget(vectorC1);
    matrixA->setChecked(true);
    left->setLayout(leftLayout);
    right->setLayout(rightLayout);
    setMatrixLayout->addWidget(left);
    setMatrixLayout->addStretch();
    setMatrixLayout->addWidget(right);
    setMatrixBox->setLayout(setMatrixLayout);

    setMatrixButtonGroup = new QButtonGroup;
    setMatrixButtonGroup->addButton(matrixA);
    setMatrixButtonGroup->addButton(matrixA1);
    setMatrixButtonGroup->addButton(matrixA2);
    setMatrixButtonGroup->addButton(matrixB2);
    setMatrixButtonGroup->addButton(vectorB1);
    setMatrixButtonGroup->addButton(vectorC1);
//  */
    startButton = new QPushButton("Start Calculatin`");

    mainLayout->addWidget(statementsBox);
    mainLayout->addWidget(setNBox);
    mainLayout->addWidget(setMatrixBox);
    mainLayout->addStretch();
    mainLayout->addWidget(startButton);
    setLayout(mainLayout);
}

MenuWidget::~MenuWidget() {}

void    MenuWidget::start(int n) {
    double x;
    MPI_Send(&n, 1, MPI_INT, MPIHandler::getDest(1) , 0, MPI_COMM_WORLD);   //  for b
    MPI_Send(&n, 1, MPI_INT, MPIHandler::getDest(2) , 0, MPI_COMM_WORLD);   //  for C2
    MPIHandler::sendVector(EnterWidget::vectorB1, n, MPIHandler::getDest(3));   //  for 26b1 - c1
    MPIHandler::sendVector(EnterWidget::vectorC1, n, MPIHandler::getDest(3));   //  for 26b2 - c2
    MPIHandler::sendMatrix(EnterWidget::matrixA, n, MPIHandler::getDest(4));    //  for y1
    MPIHandler::sendMatrix(EnterWidget::matrixA1, n, MPIHandler::getDest(5));   //  for y2
    MPIHandler::sendMatrix(EnterWidget::matrixB2, n, MPIHandler::getDest(6));   //  for (B2 + 26C2)
    MPIHandler::sendMatrix(EnterWidget::matrixA2, n, MPIHandler::getDest(7));   //  for Y3

#ifdef DEBUG_MODE
    if (!MPI_Recv(&x, 1, MPI_DOUBLE, MPIHandler::getDest(18), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE)) {
        std::cerr << "I am " << MPIHandler::getRank() << " and I received from" << MPIHandler::getDest(18) << std::endl;
    }
#else
    MPI_Recv(&x, 1, MPI_DOUBLE, MPIHandler::getDest(18), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#endif
    std::cout << x << std::endl;
}
