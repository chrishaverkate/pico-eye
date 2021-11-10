

#include <pico-eye/veml7700.h>
#include <pico-eye/tcs3471.h>

#include "ui_lcd.h"

#include <stdio.h>
#include <memory>

#include <hardware/i2c.h>
#include <pico/stdlib.h>
#include <pico/binary_info.h>

using std::make_unique;


static const int ADDRESS_LUX = 0x10;  ///< default address for VEML7700
static const int ADDRESS_RGB = 0x29;  ///< default address for TCS3471

static const uint PIN_I2C_SDA = 26;   ///< pin for i2c1 data
static const uint PIN_I2C_SCL = 27;   ///< pin for i2c1 clock

static const absolute_time_t DELAY_READ_LUX = 5000;

void read_lux_veml(i2c_inst_t* handle, Veml7700& sensor);
void configure_lux_veml(i2c_inst_t* handle, Veml7700& sensor);

void read_tcs(i2c_inst_t* handle, Tcs3471& sensor);
void configure_tcs(i2c_inst_t* handle, Tcs3471& sensor);

int main() {
	stdio_init_all();
	printf("-> Entering main!\n");

	printf("...Setting up LCD Screen\n");

	auto ui_lcd = make_unique<UiLCD>();
	ui_lcd->initialize();

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

	Tcs3471 sensor_rgb;
	configure_tcs(handle, sensor_rgb);

	Veml7700 sensor_lux;
	configure_lux_veml(handle, sensor_lux);

	static absolute_time_t last_us = get_absolute_time();

	while (true) {
		sleep_ms(10);
		absolute_time_t now_us = get_absolute_time();

		if (now_us - last_us > DELAY_READ_LUX) {

			read_lux_veml(handle, sensor_lux);

			read_tcs(handle, sensor_rgb);

			ui_lcd->update_lux(sensor_lux.get_white_channel_lux());
			ui_lcd->update_red(sensor_rgb.get_red_percent());
			ui_lcd->update_green(sensor_rgb.get_green_percent());
			ui_lcd->update_blue(sensor_rgb.get_blue_percent());
			ui_lcd->update_clear(sensor_rgb.get_clear_percent());
			ui_lcd->update_cct(sensor_rgb.calculate_cct());

			sensor_rgb.calculate_cct();
			sleep_ms(1000);
			last_us = now_us;
		}
	}
}

void read_lux_veml(i2c_inst_t* handle, Veml7700& sensor) {
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

void configure_lux_veml(i2c_inst_t* handle, Veml7700& sensor) {
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

void read_tcs(i2c_inst_t* handle, Tcs3471& sensor) {
	// Send request
	uint8_t buffer_request[1]{};

	// Issuing a read with no data will get a response of all 28 buffers
	i2c_write_blocking(handle, ADDRESS_RGB, buffer_request, 0, false);

	//Get response
	uint8_t buffer_response[30]{};
	int bytes_available = i2c_read_blocking(handle, ADDRESS_RGB, buffer_response, 28, false);

	if (bytes_available > 0) {
		sensor.cache_raw_buffers(buffer_response);
	} else {
		printf("\t ! no bytes to read.\n");
	}
}

void configure_tcs(i2c_inst_t* handle, Tcs3471& sensor) {
	printf("Configuring TCS3471...\n");
	uint8_t buffer[30]{};
	uint8_t buffer_length = 0;
	int result_write = 0;

	// set gain to 4x
	buffer[0] = 0x8F;
	buffer[1] = 0x02;
	result_write += i2c_write_blocking(handle, ADDRESS_RGB, buffer, 2, false);

	sleep_ms(1);

	sensor.get_command_config(buffer, buffer_length);
	result_write += i2c_write_blocking(handle, ADDRESS_RGB, buffer, buffer_length, false);

	// set integration time to 24ms
//	buffer[0] = 0x81;
//	buffer[1] = 0xF6;
//	result_write += i2c_write_blocking(handle, ADDRESS_RGB, buffer, 2, false);





	if(result_write > 0) {
		printf("\t Success - ");
	} else {
		printf("\t Error - ");
	}
	printf("\tBytes written: %d\n", result_write);
}
