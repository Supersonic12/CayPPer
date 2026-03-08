add_library(hyprlandchanger SHARED
  hyprlandchanger.cpp
  hyprlandchanger.h
)
target_include_directories(
  hyprlandchanger PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
install(TARGETS hyprlandchanger
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT hyprland
)
