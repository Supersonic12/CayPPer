cmake_minimum_required(VERSION 3.16)

project(CayPPer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Quick
    Gui
    Qml
    WaylandClient
)

qt_standard_project_setup(REQUIRES 6.8)
set(CMAKE_AUTORCC ON)
qt_add_executable(appCayPPer
    resources.qrc
    main.cpp
)

qt_add_qml_module(appCayPPer
    URI CayPPer
    QML_FILES
        Main.qml
    SOURCES
        src/cpp/inputtakerfromqml.h
        src/cpp/inputtakerfromqml.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appCayPPer PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appCayPPer
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
add_library(ListDirectory SHARED
    src/cpp/directorylister.h)
target_link_libraries(appCayPPer
    PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Gui
    Qt6::Qml
    Qt6::WaylandClient
)

target_include_directories(ListDirectory PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp
)

include(GNUInstallDirs)
install(TARGETS appCayPPer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
