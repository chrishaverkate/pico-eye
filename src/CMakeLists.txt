
set(target_name ${PROJECT_NAME}_lib)

add_library(
	${target_name}
	OBJECT

	veml7700.cpp
)

target_link_libraries(
	${target_name}
	PUBLIC

	${CMAKE_PROJECT_NAME}_includes
)
