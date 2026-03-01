
find_package(sdbus-c++ REQUIRED)
find_package(nlohmann_json REQUIRED)
add_library(
	plasmawallchanger SHARED
	kdechanger.h
	kdechanger.cpp
	kdemonitorlisting.h
	kdemonitorlisting.cpp
	script.js
)
target_link_libraries(plasmawallchanger
	PRIVATE
	SDBusCpp::sdbus-c++
	nlohmann_json::nlohmann_json
)
target_include_directories(plasmawallchanger
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)
