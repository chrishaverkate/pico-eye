#pragma once


#include <cstdio>
#include <stdint.h>

class UiLCD {
public:
	UiLCD();
	void initialize();
	void draw();

	void update_lux(float lux);
	void update_red(float red);
	void update_green(float green);
	void update_blue(float blue);
	void update_clear(float clear);

private:
	uint16_t* _display_buffer{};

};
