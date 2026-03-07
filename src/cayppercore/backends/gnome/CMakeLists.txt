find_package(sdbus-c++ REQUIRED)

add_library(
  gnomewallchanger SHARED
  gnomechanger.cpp
  gnomechanger.h
)

target_include_directories(gnomewallchanger
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS gnomewallchanger
LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
