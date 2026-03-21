add_library(xfcechanger MODULE
  xfcechanger.cpp
  xfcechanger.h
)
set_target_properties(xfcechanger PROPERTIES PREFIX "")
target_link_options(xfcechanger PRIVATE "-Wl,--unresolved-symbols=ignore-in-shared-libs")

install(TARGETS xfcechanger
  LIBRARY DESTINATION lib/caypper
  COMPONENT xfce
)
