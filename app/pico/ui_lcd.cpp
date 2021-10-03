
#include "ui_lcd.h"

extern "C" {
#include "lcd/lib/Config/DEV_Config.h"
#include "lcd/lib/GUI/GUI_Paint.h"
#include "lcd/lib/LCD/LCD_1in14.h"
}

UiLCD::UiLCD() {
	printf("\nUI: Creating UI LCD!\n");
}

void UiLCD::initialize() {
	printf("UI: Initializing...\n");
	sleep_ms(100);
	UDOUBLE image_size = LCD_1IN14_HEIGHT * LCD_1IN14_WIDTH * 2;
	if((_display_buffer = (UWORD*)malloc(image_size)) == NULL) {
		printf("Failed to apply for black memory...\r\n");
		while(1) ;
	}

	if(DEV_Module_Init() != 0){
		printf("! LCD: Error DEV_Module_Init()\n");
		return;
	}

	// Set brightness
	DEV_SET_PWM(50);

	/* LCD Init */
	printf("Clearing Screen...\n");
	LCD_1IN14_Init(HORIZONTAL);
	LCD_1IN14_Clear(WHITE);

	Paint_NewImage((UBYTE*)_display_buffer, LCD_1IN14.WIDTH, LCD_1IN14.HEIGHT, 0, WHITE);
	Paint_SetScale(65);
	Paint_Clear(WHITE);
	Paint_SetRotate(ROTATE_180);
	Paint_Clear(BLACK);

	update_lux(0.0f);
	update_red(0.0f);
	update_green(0.0f);
	update_blue(0.0f);
	update_clear(0.0f);

	LCD_1IN14_Display(_display_buffer);
//	sleep_ms(2000);
}

void UiLCD::draw() {
	LCD_1IN14_Display(_display_buffer);
}

void UiLCD::update_lux(uint32_t lux) {
	char buffer[30];
	snprintf(buffer, 29, "Lux: %lu", lux);
	Paint_DrawRectangle(40, 1, 150, 25, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
	Paint_DrawString_EN(1, 1, buffer, &Font24, BLACK, WHITE);
	draw();
}

void UiLCD::update_red(float red) {
	_red_light.percent_filled = red;
	draw_bar_vertical(_red_light);
}

void UiLCD::update_green(float green) {
	_green_light.percent_filled = green;
	draw_bar_vertical(_green_light);
}

void UiLCD::update_blue(float blue) {
	_blue_light.percent_filled = blue;
	draw_bar_vertical(_blue_light);
}

void UiLCD::update_clear(float clear) {
	_clear_light.percent_filled = clear;
	draw_bar_vertical(_clear_light);
}

void UiLCD::draw_bar_vertical(const Bar& bar) {
	float fill_percent = (float)bar.percent_filled / 100.0f;
	int y_start_for_fill = (bar.y_stop - bar.y_start) * fill_percent;
	Paint_DrawRectangle(bar.x_start, bar.y_start, bar.x_stop, bar.y_stop, bar.background, DOT_PIXEL_1X1, DRAW_FILL_FULL);
	Paint_DrawRectangle(bar.x_start, bar.y_start, bar.x_stop, bar.y_stop, bar.color, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
	Paint_DrawRectangle(bar.x_start, bar.y_stop - y_start_for_fill, bar.x_stop, bar.y_stop, bar.color, DOT_PIXEL_1X1, DRAW_FILL_FULL);
	draw();
}
