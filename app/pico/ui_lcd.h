#pragma once


#include <cstdio>
#include <stdint.h>

extern "C" {
#include "lcd/lib/GUI/GUI_Paint.h"
}

class UiLCD {
public:
	UiLCD();
	void initialize();
	void draw();

	void update_lux(uint32_t lux);
	void update_red(float red);
	void update_green(float green);
	void update_blue(float blue);
	void update_clear(float clear);
	void update_cct(int cct);



private:
	uint16_t* _display_buffer{};

	struct Bar {
		int x_start;
		int x_stop;
		int y_start;
		int y_stop;
		uint16_t color;
		uint16_t background;
		int percent_filled;
	};

	Bar _red_light {160, 175, 30,130, RED, BLACK, 0};
	Bar _green_light {180, 195, 30, 130, GREEN, BLACK, 0};
	Bar _blue_light{200, 215, 30, 130, BLUE, BLACK, 0};
	Bar _clear_light {220, 235, 30, 130, WHITE, BLACK, 0};

	void draw_bar_vertical(const Bar& bar);
};
