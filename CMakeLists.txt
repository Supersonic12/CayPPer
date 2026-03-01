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

option(KDE_BACKEND "KDE wayland backend for KDE environment" ON)
option(XFCE_BACKEND "XFCE backend enabled" ON)
option(GNOME_BACKEND "GNOME backend enabled" ON)
option(HYPRLAND_BACKEND "Hyprland backend enabled" ON)
option(SWAY_BACKEND "sway backend enabled" ON)
option(XELEVEN_BACKEND "X11 backend(works with xwallpaper) enabled" ON)
if(KDE_BACKEND)
    add_subdirectory(src/cayppercore/backends/kdeplasma/)
    target_link_libraries(cayppercore
    PRIVATE
    plasmawallchanger
)
endif()
if(XFCE_BACKEND)
    add_subdirectory(src/cayppercore/backends/xfce/)
    target_link_libraries(cayppercore
    PRIVATE
    xfcewallchanger
)
endif()
if(HYPRLAND_BACKEND)
    add_subdirectory(src/cayppercore/backends/hyprland/)
    target_link_libraries(cayppercore
    PRIVATE
    hyprlandchanger)
endif()
if(SWAY_BACKEND)
    add_subdirectory(src/cayppercore/backends/sway)
    target_link_libraries(cayppercore
    PRIVATE
    swaychanger)
endif()
if(XELEVEN_BACKEND)
    add_subdirectory(src/cayppercore/backends/x11)
    target_link_libraries(cayppercore
    PRIVATE
    XElevenChanger)
endif()
if(GNOME_BACKEND)
    add_subdirectory(src/cayppercore/backends/gnome)
    target_link_libraries(cayppercore
    PRIVATE
    gnomewallchanger)
endif()
include(GNUInstallDirs)
