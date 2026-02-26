
find_package(sdbus-c++ REQUIRED)
add_library(
	plasmawallchanger STATIC
	kdechanger.h
	kdechanger.cpp
	kdemonitorlisting.h
	kdemonitorlisting.cpp
)
target_link_libraries(plasmawallchanger
	PRIVATE
	SDBusCpp::sdbus-c++
)
target_include_directories(plasmawallchanger
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)
