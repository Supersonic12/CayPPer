find_package(Qt6 REQUIRED COMPONENTS Core Quick QuickControls2)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
qt_add_executable(caypper
    cpp/main.cpp
)


qt_add_qml_module(caypper
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
    qml/icons/caypper.png
    qml/icons/caypper.svg

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


target_link_libraries(caypper
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
    Qt6::QuickControls2
    cayppercore
)
install(TARGETS caypper
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT base
)
install(FILES qml/icons/caypper.png
    DESTINATION share/icons/hicolor/256x256/apps
    COMPONENT base)

install(FILES qml/icons/caypper.svg
    DESTINATION share/icons/hicolor/scalable/apps
    COMPONENT base)


