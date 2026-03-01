add_library(hyprlandchanger SHARED
  hyprlandchanger.cpp
  hyprlandchanger.h
)
target_include_directories(
  hyprlandchanger PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
