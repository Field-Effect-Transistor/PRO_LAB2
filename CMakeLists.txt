cmake_minimum_required(VERSION 3.16)

project(Lab2 LANGUAGES CXX)

# Вказати стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Знайти Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Знайти OpenMPI
find_package(MPI REQUIRED)
#find_package(Boost REQUIRED)

# Вказати ваші вихідні файли
set(SOURCES
    src/main.cpp
    src/ui/MenuWidget.cpp
    src/ui/EnterWidget.cpp
    src/ui/MainWindow.cpp
    src/ui/NotificationWidget.cpp
    src/logic/MPIHandler.cpp
    src/logic/Math.cpp
)

# Створити виконуваний файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Підключити бібліотеки
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    MPI::MPI_CXX
)

# Додати include директрії
target_include_directories(${PROJECT_NAME} PRIVATE
    ${MPI_INCLUDE_PATH}
#    ${Boost_INCLUDE_DIR}
)
