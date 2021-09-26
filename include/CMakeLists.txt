set(target_name ${CMAKE_PROJECT_NAME}_includes)

add_library(
	${target_name}
	INTERFACE
)

target_include_directories(
	${target_name}
	INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

