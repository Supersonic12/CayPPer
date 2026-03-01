add_library(
  swaychanger SHARED
  swaychanger.cpp 
  swaychanger.h
)
target_include_directories(
  swaychanger PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
