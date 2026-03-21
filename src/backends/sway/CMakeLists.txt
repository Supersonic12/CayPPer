find_package(nlohmann_json REQUIRED)
add_library(
  swaychanger MODULE
  swaychanger.cpp
  swaychanger.h
)
set_target_properties(swaychanger PROPERTIES PREFIX "")
target_link_options(swaychanger PRIVATE "-Wl,--unresolved-symbols=ignore-in-shared-libs")
target_link_libraries(swaychanger PRIVATE
  nlohmann_json::nlohmann_json)
install(TARGETS swaychanger
  LIBRARY DESTINATION lib/caypper
  COMPONENT sway
)
