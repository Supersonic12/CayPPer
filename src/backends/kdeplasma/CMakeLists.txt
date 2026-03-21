find_package(sdbus-c++ REQUIRED)
find_package(nlohmann_json REQUIRED)
add_library(
	plasmachanger MODULE
	kdechanger.h
	kdechanger.cpp
	kdemonitorlisting.h
	kdemonitorlisting.cpp
)
set_target_properties(plasmachanger PROPERTIES PREFIX "")
target_link_options(plasmachanger PRIVATE "-Wl,--unresolved-symbols=ignore-in-shared-libs")
target_link_libraries(plasmachanger
	PRIVATE
	SDBusCpp::sdbus-c++
	nlohmann_json::nlohmann_json
)
#set(MY_JS_PATH "share/caypper")
target_compile_definitions(plasmachanger PRIVATE
	JS_TEMPLATE_PATH="${CMAKE_INSTALL_PREFIX}/share/caypper/script.js")
install(FILES "script.js"
	DESTINATION share/caypper
	COMPONENT kde
)
INSTALL(TARGETS plasmachanger
	LIBRARY DESTINATION lib/caypper
	COMPONENT kde
)
