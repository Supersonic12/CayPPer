add_library(hyprlandchanger MODULE
  hyprlandchanger.cpp
  hyprlandchanger.h
)
set_target_properties(hyprlandchanger PROPERTIES PREFIX "")
target_link_options(hyprlandchanger PRIVATE "-Wl,--unresolved-symbols=ignore-in-shared-libs")
install(TARGETS hyprlandchanger
  LIBRARY DESTINATION lib/caypper
  COMPONENT hyprland
)

