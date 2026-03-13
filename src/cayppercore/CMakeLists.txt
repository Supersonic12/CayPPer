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
    changerFactory.cpp
    changerFactory.h
    domainExpansion/raiiguard.h
    persistenceaddon.h persistenceaddon.cpp
    domainExpansion/monState.h


)

target_include_directories(cayppercore
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS cayppercore
    LIBRARY DESTINATION lib/caypper
    COMPONENT base
)
target_link_libraries(cayppercore
PRIVATE
${CMAKE_DL_LIBS}
)
