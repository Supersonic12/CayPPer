add_library(
  swaychanger MODULE
  swaychanger.cpp
  swaychanger.h
)
set_target_properties(swaychanger PROPERTIES PREFIX "")
target_link_options(swaychanger PRIVATE "-Wl,--unresolved-symbols=ignore-in-shared-libs")
install(TARGETS swaychanger
  LIBRARY DESTINATION lib/caypper
  COMPONENT sway
)
