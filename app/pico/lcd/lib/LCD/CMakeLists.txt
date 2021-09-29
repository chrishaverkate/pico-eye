add_library(LCD
	STATIC
	LCD_1in14.c
	LCD_1in14.h
)

target_link_libraries(LCD
	PUBLIC
	Config
)
