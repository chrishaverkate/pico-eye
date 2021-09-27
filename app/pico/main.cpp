
#include <pico/binary_info.h>
#include <pico/stdlib.h>
#include <pico-eye/veml7700.h>
#include <stdio.h>

#include <hardware/i2c.h>
#include <pico/stdlib.h>

static const int ADDRESS_LUX = 0x10;  ///< default address for VEML7700
static const uint PIN_I2C_SDA = 26;   ///< pin for i2c1 data
static const uint PIN_I2C_SCL = 27;   ///< pin for i2c1 clock

static const absolute_time_t DELAY_READ_LUX = 2000000;

void read_lux(i2c_inst_t* handle, Veml7700& sensor);
void configure_lux(i2c_inst_t* handle, Veml7700& sensor);

int main() {
	stdio_init_all();
	puts("-> Entering main!");

	puts("...Setting up i2c");

	i2c_inst_t* handle = i2c1;
	uint init_result = i2c_init(handle, 400 * 1000);  // 400kHz

	if (init_result > 0) {
		gpio_set_function(PIN_I2C_SDA, GPIO_FUNC_I2C);
		gpio_set_function(PIN_I2C_SCL, GPIO_FUNC_I2C);
		gpio_pull_up(PIN_I2C_SDA);  // should need to set the pull-ups for slave device
		gpio_pull_up(PIN_I2C_SCL);
		printf("! Finished setting up i2c. Handle: %p\n", handle);
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
//			printf("Read...\n");
			read_lux(handle, sensor);
			printf(" -> %.2f lux", sensor.get_white_channel_lux());
			last_us = now_us;
		}
	}
}

void read_lux(i2c_inst_t* handle, Veml7700& sensor) {
//	printf("\nReading light...\n");
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
//		printf("\t -> ");
//		for (int i(0); i < bytes_available; ++i) {
//			printf("%x ", buffer_response[i]);
//		}
		sensor.cache_raw_results_white_channel(buffer_response);
	} else {
		printf("\t ! no bytes to read.\n");
	}
}

void configure_lux(i2c_inst_t* handle, Veml7700& sensor) {
	printf("\nConfiguring...\n");

	uint8_t buffer[30]{};
	uint8_t buffer_length = 0;
	sensor.get_command_config(buffer, buffer_length);
	int result_write = i2c_write_blocking(handle, ADDRESS_LUX, buffer, buffer_length, false);

	printf("\tBytes written: %d\n", result_write);
}
