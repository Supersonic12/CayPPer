cmake_minimum_required(VERSION 3.16)

project(CayPPer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Quick
    Qml
)

qt_standard_project_setup(REQUIRES 6.8)
option(KDEBACKEND "enable KDE Backend with its dependencies requirements" OFF)
include(GNUInstallDirs)
add_subdirectory(src)


install(FILES caypper.desktop
    DESTINATION share/applications
    COMPONENT base)
install(FILES uninstall.sh
    DESTINATION share/caypper
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
    GROUP_EXECUTE GROUP_READ
    WORLD_EXECUTE WORLD_READ
    COMPONENT base
)

target_compile_definitions(cayppercore
    PRIVATE
    LIB_DIR="${CMAKE_INSTALL_PREFIX}/"
)
