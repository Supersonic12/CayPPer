add_library(
  xelevenchanger MODULE
  changerX.cpp
  changerX.h
)
set_target_properties(xelevenchanger PROPERTIES PREFIX "")
target_link_options(xelevenchanger PRIVATE "-Wl,--unresolved-symbols=ignore-in-shared-libs")
install(TARGETS xelevenchanger
  LIBRARY DESTINATION lib/caypper
  COMPONENT x11
)
