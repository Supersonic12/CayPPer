add_library(cayppercore STATIC
    directorylister.cpp
    directorylister.h
    changer.cpp
    changer.h
    composfinder.cpp
    composfinder.h
    monitorhandler.h
    monitorhandler.cpp
    coreservice.h
    coreservice.cpp
    domainExpansion/fillmode.h
    domainExpansion/fillmodeconverter.h

)

target_include_directories(cayppercore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
