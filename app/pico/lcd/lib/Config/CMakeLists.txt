add_library(Config
	STATIC
	Debug.h
	DEV_Config.c
	DEV_Config.h
)

target_link_libraries(Config
	PUBLIC
	pico_stdlib
	hardware_spi
	hardware_i2c
	hardware_pwm
	hardware_adc
)
