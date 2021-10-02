
#include <cstring>
#include <pico-eye/tcs3471.h>

bool Tcs3471::get_command_config(uint8_t* out_buffer, uint8_t& out_buffer_length) {
	uint8_t message = 0;

	message |= 0x02; // Enable RGBC ADCs
	message |= 0x01; // Power ON

	out_buffer[0] = REG_ENABLE;
	out_buffer[1] = message;

	out_buffer_length = 2;

	return true;
}

float Tcs3471::get_white_channel_lux() const {
	return 0.0f;
}

void Tcs3471::cache_raw_buffers(const uint8_t* results) {
	memcpy(_raw_results, results, 28);
}

uint16_t Tcs3471::get_max_count() const {
	uint16_t count = 0;

	switch (_raw_results[REG_RGBC_TIMING]) {
		case 0xFF: count = 1024; break;
		case 0xF6: count = 10240; break;
		case 0xD5: count = 43008; break;
		case 0xC0: count = 65535; break;
		case 0x00: count = 65535; break;
	}

	return count;
}
float Tcs3471::get_red_percent() const {
	uint16_t red = _raw_results[REG_DATA_HIGH_RED] << 8 | _raw_results[REG_DATA_LOW_RED];
	return static_cast<float>(red) / static_cast<float>(get_max_count()) * 100.0;
}

float Tcs3471::get_green_percent() const {
	uint16_t green = _raw_results[REG_DATA_HIGH_GREEN] << 8 | _raw_results[REG_DATA_LOW_GREEN];
	return static_cast<float>(green) / static_cast<float>(get_max_count()) * 100.0;
}

float Tcs3471::get_blue_percent() const {
	uint16_t blue = _raw_results[REG_DATA_HIGH_BLUE] << 8 | _raw_results[REG_DATA_LOW_BLUE];
	return static_cast<float>(blue) / static_cast<float>(get_max_count()) * 100.0;
}

float Tcs3471::get_clear_percent() const {
	uint16_t clear = _raw_results[REG_DATA_HIGH_CLEAR] << 8 | _raw_results[REG_DATA_LOW_CLEAR];
	return static_cast<float>(clear) / static_cast<float>(get_max_count()) * 100.0;
}

uint16_t Tcs3471::get_saturation_count() const {
	// ATIME -> RGBC Timing Reg (0x01)
	// ATIME = 256 - Integration Time / 2.4 ms
	// Integration Time = 2.4 ms * (256 - ATIME)
	// Saturation = ((Integration Time / 2.7) * 1024) - 1

	uint16_t saturation = (static_cast<uint16_t>(_raw_results[REG_RGBC_TIMING] / 2.7) * 1024) - 1;
	uint16_t max_count = (256 - _raw_results[REG_RGBC_TIMING]) * 1024;
	return saturation;
}
