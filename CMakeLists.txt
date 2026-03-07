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
include(GNUInstallDirs)
add_subdirectory(src/cayppercore)
add_subdirectory(src/caypperqt)

option(KDE_BACKEND "KDE wayland backend for KDE environment" OFF)
option(XFCE_BACKEND "XFCE backend enabled" OFF)
option(GNOME_BACKEND "GNOME backend enabled" OFF)
option(HYPRLAND_BACKEND "Hyprland backend enabled" OFF)
option(SWAY_BACKEND "sway backend enabled" OFF)
option(XELEVEN_BACKEND "X11 backend(works with xwallpaper) enabled" OFF)
if(KDE_BACKEND)
    add_subdirectory(src/cayppercore/backends/kdeplasma/)
    target_link_libraries(cayppercore
        PRIVATE
        plasmawallchanger
    )

target_compile_definitions(cayppercore PRIVATE KDE_BACKEND) 
endif()
if(XFCE_BACKEND)
    add_subdirectory(src/cayppercore/backends/xfce/)
    target_link_libraries(cayppercore
        PRIVATE
        xfcewallchanger
    )
target_compile_definitions(cayppercore PRIVATE XFCE_BACKEND)
endif()
if(HYPRLAND_BACKEND)
    add_subdirectory(src/cayppercore/backends/hyprland/)
    target_link_libraries(cayppercore
        PRIVATE
        hyprlandchanger)
    target_compile_definitions(cayppercore PRIVATE HYPRLAND_BACKEND)
endif()
if(SWAY_BACKEND)
    add_subdirectory(src/cayppercore/backends/sway)
    target_link_libraries(cayppercore
        PRIVATE
        swaychanger)
    target_compile_definitions(cayppercore PRIVATE SWAY_BACKEND)
endif()
if(XELEVEN_BACKEND)
    add_subdirectory(src/cayppercore/backends/x11)
    target_link_libraries(cayppercore
        PRIVATE
        xelevenchanger)
    target_compile_definitions(cayppercore PRIVATE XELEVEN_BACKEND)
endif()
if(GNOME_BACKEND)
    add_subdirectory(src/cayppercore/backends/gnome)
    target_link_libraries(cayppercore
        PRIVATE
        gnomewallchanger)
    target_compile_definitions(cayppercore PRIVATE GNOME_BACKEND)
endif()

install(FILES uninstall.sh
DESTINATION share/caypper
PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
GROUP_EXECUTE GROUP_READ
WORLD_EXECUTE WORLD_READ
)

