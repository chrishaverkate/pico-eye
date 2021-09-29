set(target_name pico-eye)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

include_directories(${CMAKE_CURRENT_LIST_DIR}/lcd/lib/Config)
include_directories(${CMAKE_CURRENT_LIST_DIR}/lcd/lib/GUI)
include_directories(${CMAKE_CURRENT_LIST_DIR}/lcd/lib/LCD)

add_subdirectory(lcd)

add_executable(${target_name}
	main.cpp
)

set_target_properties(${target_name}
	PROPERTIES
		C_STANDARD 11
		CXX_STANDARD 17
)

target_link_libraries(${target_name}
	pico_stdlib
	hardware_i2c
	hardware_spi
	${PROJECT_NAME}_lib

	# Screen libraries
	Config
	LCD
	GUI
	Fonts
)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${target_name})

# Workaround for getting bin file to be copied from docker env when automated
add_custom_command(
	TARGET ${target_name}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${target_name}> ${PRJ_APPS_OUTPUT_DIR}
)
