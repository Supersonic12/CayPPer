add_library(
  xelevenchanger SHARED
  changerX.cpp
  changerX.h
)
target_include_directories(xelevenchanger
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS xelevenchanger
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
