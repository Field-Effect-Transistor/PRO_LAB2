cmake_minimum_required(VERSION 3.16)

project(Lab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

if (MPI_ON)
    message("MPI ON")
    find_package(MPI REQUIRED)
    add_compile_options(-DMPI_ON)
    set(SOURCES
        src/main.cpp
        src/ui/MenuWidget.cpp
        src/ui/EnterWidget.cpp
        src/ui/MainWindow.cpp
        src/ui/NotificationWidget.cpp
        src/logic/Math.cpp
        src/logic/MPIHandler.cpp
    )

    add_executable(${PROJECT_NAME} ${SOURCES})
    
    target_link_libraries(${PROJECT_NAME}
        MPI::MPI_CXX
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
    )
else()
    set(SOURCES
        src/main.cpp
        src/ui/MenuWidget.cpp
        src/ui/EnterWidget.cpp
        src/ui/MainWindow.cpp
        src/ui/NotificationWidget.cpp
        src/logic/Math.cpp
    )

    add_executable(${PROJECT_NAME} ${SOURCES})

    target_link_libraries(${PROJECT_NAME}
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
    )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${MPI_INCLUDE_PATH}
)

if (DEBUG_MODE)
    message(STATUS "DEBUG MODE ON")
    add_compile_definitions(-DDEBUG_MODE)
endif()

if (VERBOSE_MODE)
    message(STATUS "VERBOSE MODE ON")
    add_compile_definitions(-DVERBOSE_MODE)
endif()

#set(MAXN 1000 CACHE STRING "Max value of N")
if (MAXN)
    message(STATUS "Max N set to ${MAXN}")
    add_compile_definitions(MAXN=${MAXN})
endif()