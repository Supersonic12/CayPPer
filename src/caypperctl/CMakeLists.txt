find_package(Qt6 REQUIRED COMPONENTS Core)
qt_add_executable(caypperctl
  main.cpp
  controller.h controller.cpp

)
target_link_libraries(caypperctl
  PRIVATE
  Qt6::Core
  cayppercore
)
