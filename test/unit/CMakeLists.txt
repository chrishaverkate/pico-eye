set(target_name unit-tests)

add_executable(${target_name}
	tcs3471_tests.cpp
	veml7700_tests.cpp
)

set_target_properties(${target_name}
	PROPERTIES
	C_STANDARD 11
	CXX_STANDARD 17
)

target_link_libraries(${target_name}
	${PROJECT_NAME}_lib
	gtest_main
)

gtest_discover_tests(${target_name})
