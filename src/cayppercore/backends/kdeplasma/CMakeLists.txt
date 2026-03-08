find_package(sdbus-c++ REQUIRED)
find_package(nlohmann_json REQUIRED)
add_library(
	plasmawallchanger SHARED
	kdechanger.h
	kdechanger.cpp
	kdemonitorlisting.h
	kdemonitorlisting.cpp
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
#set(MY_JS_PATH "share/caypper")
target_compile_definitions(plasmawallchanger PRIVATE
	JS_TEMPLATE_PATH="${CMAKE_INSTALL_PREFIX}/share/caypper/script.js")
install(FILES "script.js"
	DESTINATION share/caypper
	COMPONENT kde
)
INSTALL(TARGETS plasmawallchanger
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	COMPONENT kde
)
