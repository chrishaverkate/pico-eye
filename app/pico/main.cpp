
#include <pico/binary_info.h>
#include <pico/stdlib.h>
#include <pico-eye/veml7700.h>
#include <stdio.h>

#include <hardware/i2c.h>
#include <pico/stdlib.h>

extern "C" {
#include "lcd/lib/Config/DEV_Config.h"
#include "lcd/lib/GUI/GUI_Paint.h"
#include "lcd/lib/LCD/LCD_1in14.h"
}

static const int ADDRESS_LUX = 0x10;  ///< default address for VEML7700
static const uint PIN_I2C_SDA = 26;   ///< pin for i2c1 data
static const uint PIN_I2C_SCL = 27;   ///< pin for i2c1 clock

static const absolute_time_t DELAY_READ_LUX = 1000000;

void read_lux(i2c_inst_t* handle, Veml7700& sensor);
void configure_lux(i2c_inst_t* handle, Veml7700& sensor);

void init_lcd(UWORD*& display_buffer);
void update_lux(UWORD* display_buffer, float lux);

int main() {
	stdio_init_all();
	printf("-> Entering main!\n");

	printf("...Setting up LCD Screen\n");
	UWORD* display_buffer;
	init_lcd(display_buffer);

	printf("...Setting up i2c\n");
	i2c_inst_t* handle = i2c1;
	uint init_result = i2c_init(handle, 400 * 1000);  // 400kHz

	if (init_result > 0) {
		gpio_set_function(PIN_I2C_SDA, GPIO_FUNC_I2C);
		gpio_set_function(PIN_I2C_SCL, GPIO_FUNC_I2C);
		gpio_pull_up(PIN_I2C_SDA);  // should need to set the pull-ups for slave device
		gpio_pull_up(PIN_I2C_SCL);
		printf("Finished setting up i2c. Handle: %p\n", handle);
	} else {
		printf("! Error setting up i2c: %u\n", init_result);
	}

	Veml7700 sensor;
	configure_lux(handle, sensor);

	static absolute_time_t last_us = get_absolute_time();

	while (true) {
		sleep_ms(100);
		absolute_time_t now_us = get_absolute_time();

		if (now_us - last_us > DELAY_READ_LUX) {
			read_lux(handle, sensor);
			update_lux(display_buffer, sensor.get_white_channel_lux());
			printf("%.2f lux\n", sensor.get_white_channel_lux());
			last_us = now_us;
		}
	}
}

void read_lux(i2c_inst_t* handle, Veml7700& sensor) {
	// Send request
	uint8_t buffer_request[10]{};
	uint8_t buffer_length = 0;
	sensor.get_command_white_channel_output(buffer_request, buffer_length);
	i2c_write_blocking(handle, ADDRESS_LUX, buffer_request, 1, true);
	i2c_write_raw_blocking(handle, buffer_request + 1, 1);

	//Get response
	uint8_t buffer_response[30]{};
	int bytes_available = i2c_read_blocking(handle, ADDRESS_LUX, buffer_response, 2, false);

	if (bytes_available > 0) {
		sensor.cache_raw_results_white_channel(buffer_response);
	} else {
		printf("\t ! no bytes to read.\n");
	}
}

void configure_lux(i2c_inst_t* handle, Veml7700& sensor) {
	printf("Configuring VEML7700...\n");

	uint8_t buffer[30]{};
	uint8_t buffer_length = 0;
	sensor.get_command_config(buffer, buffer_length);
	int result_write = i2c_write_blocking(handle, ADDRESS_LUX, buffer, buffer_length, false);

	if(result_write > 0) {
		printf("\t Success - ");
	} else {
		printf("\t Error - ");
	}
	printf("\tBytes written: %d\n", result_write);
}

void init_lcd(UWORD*& display_buffer) {
	UDOUBLE image_size = LCD_1IN14_HEIGHT * LCD_1IN14_WIDTH * 2;
	if((display_buffer = (UWORD*)malloc(image_size)) == NULL) {
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

	Paint_NewImage((UBYTE*)display_buffer, LCD_1IN14.WIDTH, LCD_1IN14.HEIGHT, 0, WHITE);
	Paint_SetScale(65);
	Paint_Clear(WHITE);
	Paint_SetRotate(ROTATE_270);
	Paint_Clear(BLACK);

	Paint_DrawString_EN(1, 1, "Lux:", &Font24, BLACK, WHITE);
	LCD_1IN14_Display(display_buffer);
	sleep_ms(2000);
}

void update_lux(UWORD* display_buffer, float lux) {
	Paint_Clear(BLACK);
	char buffer[30];
	snprintf(buffer, 29, "%.2f", lux);
	Paint_DrawString_EN(1, 1, "Lux:", &Font24, BLACK, WHITE);
	Paint_DrawString_EN(1, 24, buffer, &Font24, BLACK, WHITE);
	LCD_1IN14_Display(display_buffer);
}
