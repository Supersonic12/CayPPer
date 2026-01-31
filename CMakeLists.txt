cmake_minimum_required(VERSION 3.16)

project(CayPPer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Quick
    Qml
)

qt_standard_project_setup(REQUIRES 6.8)

add_subdirectory(src/cayppercore)
add_subdirectory(src/caypperqt)
add_subdirectory(src/caypperwx)
