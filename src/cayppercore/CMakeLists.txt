add_library(cayppercore STATIC
    directorylister.cpp
    directorylister.h
    changer.cpp
    changer.h
    composfinder.cpp
    composfinder.h
)

target_include_directories(cayppercore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
