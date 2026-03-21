add_library(
  gnomechanger MODULE
  gnomechanger.cpp
  gnomechanger.h
)
set_target_properties(gnomechanger PROPERTIES PREFIX "")

target_link_options(gnomechanger PRIVATE "-Wl,--unresolved-symbols=ignore-in-shared-libs")

install(TARGETS gnomechanger
  LIBRARY DESTINATION lib/caypper
  COMPONENT gnome
)
