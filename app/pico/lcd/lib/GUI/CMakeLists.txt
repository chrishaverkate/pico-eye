add_library(GUI
	STATIC
	GUI_Paint.c
	GUI_Paint.h
)

target_link_libraries(GUI
	PUBLIC
	Config
)
