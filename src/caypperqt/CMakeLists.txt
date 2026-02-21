find_package(Qt6 REQUIRED COMPONENTS Core Quick Svg)

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
    qml/SettingsStack.qml
    qml/Grid.qml
    RESOURCES
    qml/icons/search.svg
    qml/icons/settings.svg
    qml/icons/folder.svg
    qml/icons/left-arrow.svg
    qml/icons/check.svg

    SOURCES
        #cpp/inputtakerfromqml.cpp
        cpp/imagemodel.cpp
        #cpp/inputtakerfromqml.h
        cpp/imagemodel.h
        cpp/controller.h
        cpp/controller.cpp
        cpp/appsettings.h
        cpp/appsettings.cpp

)


target_link_libraries(appCayPPer
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
        Qt6::Qml
        Qt6::Svg
        cayppercore
)



