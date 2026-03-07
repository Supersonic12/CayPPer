add_library(
  XElevenChanger SHARED
  changerX.cpp
  changerX.h
)
target_include_directories(XElevenChanger
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS XElevenChanger
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
