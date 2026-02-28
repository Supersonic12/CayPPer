find_package(sdbus-c++ REQUIRED)

add_library(cayppercore STATIC
    directorylister.cpp
    directorylister.h
    changer.cpp
    changer.h
    envvardetector.cpp
    envvardetector.h
    monitorlister.h
    monitorlister.cpp
    coreservice.h
    coreservice.cpp
    domainExpansion/fillmode.h
    domainExpansion/fillmodeconverter.h
    xfce/xfcechanger.h xfce/xfcechanger.cpp
    domainExpansion/jsscriptloader.h
)
target_link_libraries(cayppercore
    PRIVATE
    SDBusCpp::sdbus-c++
    plasmawallchanger)
target_include_directories(cayppercore
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
add_subdirectory(kdeplasma/)
