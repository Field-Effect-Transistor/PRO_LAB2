//  MenuWidget.cpp

#include "MenuWidget.hpp"

#ifndef MAXN
    #define MAXN 1000
#endif

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
    setNSpinBox->setValue(10);
    setNSpinBox->setMaximum(MAXN);
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
    auto start = std::chrono::high_resolution_clock::now();
#ifdef  MPI_ON
    bool isExit = false;
    for (int i = 0; i < MPIHandler::getSize(); i++) {
        MPI_Send(&isExit, 1, MPI_CXX_BOOL, i, 0, MPI_COMM_WORLD);
    }

    double x;
    //std::ostream& stream = std::cout;
    std::string path = "result_" + std::to_string(MPIHandler::getRank()) + '_' + std::to_string(((int)time(NULL) >> 2) % 1000);
    std::fstream stream(path, std::ios::out | std::ios::app);
#else
    std::fstream stream("result.txt", std::ios::out | std::ios::app);
#endif

    stream << "Matrix A:" << std::endl;
    Math::print(stream, EnterWidget::matrixA, n);

    stream << "Matrix A1:" << std::endl;    
    Math::print(stream, EnterWidget::matrixA1, n);

    stream << "Matrix A2:" << std::endl;    
    Math::print(stream, EnterWidget::matrixA2, n);    

    stream << "Matrix B2:" << std::endl;
    Math::print(stream, EnterWidget::matrixB2, n);

    stream << "Vector b1:" << std::endl;
    Math::print(stream, EnterWidget::vectorB1, n);

    stream << "Vector c1:" << std::endl;
    Math::print(stream, EnterWidget::vectorC1, n);

#ifdef  MPI_ON
    MPIHandler::sendMatrix(EnterWidget::matrixA, n, 1);
    MPIHandler::sendMatrix(EnterWidget::matrixB2, n, 2);
    MPIHandler::sendMatrix(EnterWidget::matrixA2, n, 2);

    double* vector26b1 = Math::multiply(EnterWidget::vectorB1, 26, n);
    double* vector26b1_c1 = Math::sub(vector26b1, EnterWidget::vectorC1, n);

    double* y2 = Math::multiply(EnterWidget::matrixA1, vector26b1_c1, n);

    double** Y3 = MPIHandler::receiveMatrix(n, 2);


    double* y1 = MPIHandler::receiveVector(n, 1);

    MPIHandler::sendVector(y2, n, 1);
    MPIHandler::sendVector(y2, n, 2);

    double* Y3_y2 = Math::multiply(Y3, y2, n);

    double* Y3_y2_y1 = Math::add(Y3_y2, y1, n);
    
    double* y1_y2_Y3_2_y2 = MPIHandler::receiveVector(n, 2);

    double* right = Math::add(Y3_y2_y1, y1_y2_Y3_2_y2, n);
    double* left = MPIHandler::receiveVector(n, 1);

    x = Math::multiply(left, right, n);
    
    stream << "Vector 26*b1-c1:" << std::endl;
    Math::print(stream, vector26b1_c1, n);
    stream << "Vector y2:" << std::endl;
    Math::print(stream, y2, n);
    stream << "Matrix Y3 * y2:" << std::endl;
    Math::print(stream, Y3_y2, n);
    stream << "Vector Y3*y2+y1:" << std::endl;
    Math::print(stream, Y3_y2_y1, n);
    stream << "Vector Y3 * y2 + y1 + y1` * Y3 * y1 + y2`:" << std::endl;
    Math::print(stream, right, n);

    Math::deleteVector(vector26b1_c1);
    Math::deleteVector(vector26b1);
    Math::deleteVector(y2);
    Math::deleteMatrix(Y3, n);
    Math::deleteVector(Y3_y2);
    Math::deleteVector(y1);
    Math::deleteVector(Y3_y2_y1);
    Math::deleteVector(y1_y2_Y3_2_y2);
    Math::deleteVector(left);
    Math::deleteVector(right);

#else
    double x;

    //  for b
    double* vectorB = Math::createVector(n);
    for (int i = 0; i < n; ++i) {
        vectorB[i] = 26 * pow(i + 1, 3);
    }
    stream << "Vector b:" << std::endl;
    Math::print(stream, vectorB, n);

    //  for C2
    double** matrixC2 = Math::createMatrix(n);
    for(int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            matrixC2[i][j] = 1 / (pow(i + 1, 2) + j + 1);
        }
    }
    stream << "Matrix C2:" << std::endl;
    Math::print(stream, matrixC2, n);

    //  26b1 - c1
    double* vector26b1 = Math::multiply(EnterWidget::vectorB1, 26, n);
    double* vector26b1_c1 = Math::sub(vector26b1, EnterWidget::vectorC1, n);
    Math::deleteVector(vector26b1);
    stream << "Vector 26*b1-c1:" << std::endl;
    Math::print(stream, vector26b1_c1, n);

    //  y1
    double* y1 = Math::multiply(EnterWidget::matrixA, vectorB, n);
    stream << "Vector y1:" << std::endl;
    Math::print(stream, y1, n);

    //  for y2
    double* y2 = Math::multiply(EnterWidget::matrixA1, vector26b1_c1, n);
    Math::deleteVector(vector26b1_c1);
    stream << "Vector y2:" << std::endl;
    Math::print(stream, y2, n);

    //  for B2 + 26C2
    double** matrix26C2 = Math::multiply(matrixC2, 26, n);
    double** matrixB2_26C2 = Math::add(EnterWidget::matrixB2, matrix26C2, n);
    Math::deleteMatrix(matrix26C2, n);
    stream << "Matrix B2 + 26C2:" << std::endl;
    Math::print(stream, matrixB2_26C2, n);

    //  for Y3
    double** Y3 = Math::multiply(EnterWidget::matrixA2, matrixB2_26C2, n);
    Math::deleteMatrix(matrixB2_26C2, n);
    stream << "Matrix Y3:" << std::endl;
    Math::print(stream, Y3, n);

    //  for Y3_2
    double** Y3_2 = Math::multiply(Y3, Y3, n);
    stream << "Matrix Y3_2:" << std::endl;
    Math::print(stream, Y3_2, n);

    //  for y1` * Y3
    double* y1_Y3 = Math::multiply(y1, Y3, n);
    stream << "Vector y1` * Y3:" << std::endl;
    Math::print(stream, y1_Y3, n);

    //  for Y3 * y2
    double* Y3_y2 = Math::multiply(Y3, y2, n);
    stream << "Vector Y3 * y2:" << std::endl;
    Math::print(stream, Y3_y2, n);

    //  for y2` * Y3_2
    double* y2_Y3_2 = Math::multiply(y2, Y3_2, n);
    Math::deleteMatrix(Y3_2, n);
    stream << "Vector y2` * Y3_2:" << std::endl;
    Math::print(stream, y2_Y3_2, n);

    //  for y1` * Y3 * y1
    double y1_Y3_y1 = Math::multiply(y1_Y3, y1, n);
    Math::deleteVector(y1_Y3);
    stream << "Scalar y1` * Y3 * y1:\t" << y1_Y3_y1 << std::endl;

    //  for Y3 * y2 + y1
    double* Y3_y2_y1 = Math::add(Y3_y2, y1, n);
    stream << "Vector Y3 * y2 + y1:" << std::endl;
    Math::print(stream, Y3_y2_y1, n);
    Math::deleteVector(Y3_y2);

    //  for y2` * Y3 * y2
    double y2_Y3_2_y2= Math::multiply(y2_Y3_2, y2, n);
    Math::deleteVector(y2_Y3_2);
    stream << "Scalar y2` * Y3 * y2:\t" << y2_Y3_2_y2 << std::endl;

    //  for y1` * Y3 * y1 + y2`
    double* left = Math::createVector(n);
    for (int i = 0; i < n; i++) {
        left[i] = y1_Y3_y1 + y2[i];
    }
    stream << "Vector y1` * Y3 * y1 + y2`:" << std::endl;
    Math::print(stream, left, n);

    //  for y1 * (y2` * Y3_2 * y2)
    double* y1_y2_Y3_2_y2 = Math::multiply(y1, y2_Y3_2_y2, n);
    stream << "Vector y1 * (y2` * Y3_2 * y2):" << std::endl;
    Math::print(stream, y1_y2_Y3_2_y2, n);

    //  for Y3 * y2 + y1 + y1` * Y3 * y1 + y2`
    double* right = Math::add(Y3_y2_y1, y1_y2_Y3_2_y2, n);
    stream << "Vector Y3 * y2 + y1 + y1` * Y3 * y1 + y2`:" << std::endl;
    Math::print(stream, right, n);

    //  for x
    x = Math::multiply(left, right, n);
    Math::deleteVector(right);
    Math::deleteVector(left);
    Math::deleteVector(y1);
    Math::deleteVector(y2);
    Math::deleteVector(vectorB);
    Math::deleteMatrix(matrixC2, n);
    Math::deleteMatrix(Y3, n);
#endif
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    stream << "Elapsed time: " << duration.count() << " seconds" << std::endl << std::endl;

    NotificationWidget notification("x = " + QString::number(x, 'g', 17));
    stream << "x = " << x << std::endl;
    notification.exec();
    stream.close();
}
