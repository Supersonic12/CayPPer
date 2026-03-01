add_library(xfcewallchanger SHARED
	xfcechanger.cpp
	xfcechanger.h
)
target_include_directories(xfcewallchanger
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)


