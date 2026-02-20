find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(appCayPPer
    cpp/main.cpp
)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

qt_add_qml_module(appCayPPer
    URI CayPPer
    QML_FILES
    qml/Main.qml

    qml/Grid.qml
    RESOURCES
    qml/icons/search-24x24.png
    qml/icons/search-32x32.png
    qml/icons/search-48x48.png
    qml/icons/settings-512x512.png
    qml/icons/folder-32x32.png
    qml/icons/left-arrow-32x32.png
    qml/icons/check-32x32.png

    SOURCES
        #cpp/inputtakerfromqml.cpp
        cpp/imagemodel.cpp
        #cpp/inputtakerfromqml.h
        cpp/imagemodel.h
        cpp/controller.h
        cpp/controller.cpp
        SOURCES cpp/appsettings.h cpp/appsettings.cpp
        QML_FILES qml/SettingsStack.qml
)


target_link_libraries(appCayPPer
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
        Qt6::Qml
        cayppercore
)
target_link_libraries(appCayPPer PRIVATE Qt6::Widgets)
#
#set_target_properties(appCayPPer PROPERTIES
#    MACOSX_BUNDLE TRUE
#    WIN32_EXECUTABLE TRUE
#)


