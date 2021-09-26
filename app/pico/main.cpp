
#include <pico/binary_info.h>
#include <pico/stdlib.h>
#include <project-template/delay_incrementer.h>
#include <stdio.h>

#include <hardware/i2c.h>
#include <pico/stdlib.h>

static const int ADDRESS_LUX = 0x10;  ///< default address for VEML7700
static const uint PIN_I2C_SDA = 26;   ///< pin for i2c1 data
static const uint PIN_I2C_SCL = 27;   ///< pin for i2c1 clock

static const absolute_time_t DELAY_READ_LUX = 2000000;

void read_lux(i2c_inst_t* handle);
void configure_lux(i2c_inst_t* handle);

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

	configure_lux(handle);

	static absolute_time_t last_us = get_absolute_time();

	while (true) {
		sleep_ms(100);
		absolute_time_t now_us = get_absolute_time();

		if (now_us - last_us > DELAY_READ_LUX) {
//			printf("Read...\n");
			read_lux(handle);
			last_us = now_us;
		}
	}
}

void read_lux(i2c_inst_t* handle) {
	printf("\nReading light...\n");
	// Send request
	uint8_t buffer_request[30] = {0x05, 0x20 | 0x01};
	int result_write = i2c_write_blocking(handle, ADDRESS_LUX, buffer_request, 1, true);
	i2c_write_raw_blocking(handle, buffer_request + 1, 1);

//	printf("\tBytes written: %d\n", result_write);

	sleep_ms(102);

	//Get response
	uint8_t buffer[30]{};
	int bytes_available = 0;
//	bytes_available = i2c_get_read_available(handle);

	bytes_available = i2c_read_blocking(handle, ADDRESS_LUX, buffer, 2, false);

	if (bytes_available > 0) {
//		i2c_read_raw_blocking(handle, buffer, bytes_available);
//		printf("\tBytes received: %d\n", bytes_available);
		printf("\t -> ");
		for (int i(0); i < bytes_available; ++i) {
			printf("%x ", buffer[i]);
		}

		uint16_t raw = buffer[1] << 8 | buffer[0];
		float lux = static_cast<float>(raw) * 0.1152f;
		printf(" -> %.2f lux", lux);

		printf("\n");
	} else {
		printf("\t ! no bytes to read.\n");
	}
}

void configure_lux(i2c_inst_t* handle) {
	printf("\nConfiguring...\n");

	uint16_t packed_message = 0;
//	packed_message |= 0x00 << 12; // ALS Gain x 1
	packed_message |= 0x01 << 9;  // ALS Integration time 25ms

	auto message_0 = static_cast<uint8_t>(packed_message & 0xf);
	auto message_1 = static_cast<uint8_t>(packed_message >> 8);

	uint8_t buffer_request[30] = {0x00, message_0, message_1};
	int result_write = i2c_write_blocking(handle, ADDRESS_LUX, buffer_request, 3, false);

	printf("\tBytes written: %d\n", result_write);
}
