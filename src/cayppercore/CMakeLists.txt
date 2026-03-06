add_library(cayppercore SHARED
    directorylister.cpp
    directorylister.h
    envvardetector.cpp
    envvardetector.h


    coreservice.h
    coreservice.cpp
    domainExpansion/fillmode.h
    domainExpansion/fillmodeconverter.h
    domainExpansion/jsscriptloader.h
    IChanger.h
    backends/changerFactory.cpp
    backends/changerFactory.h
    domainExpansion/raiiguard.h
)

target_include_directories(cayppercore
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

