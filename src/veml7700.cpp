
#include <pico-eye/veml7700.h>

bool Veml7700::get_command_config(uint8_t* out_buffer, uint8_t& out_buffer_length) {
	uint16_t packed_message = 0;

	//	packed_message |= 0x00 << 12; // ALS Gain x 1
	packed_message |= 0x01 << 9;  // ALS Integration time 25ms

	auto message_0 = static_cast<uint8_t>(packed_message & 0xf);
	auto message_1 = static_cast<uint8_t>(packed_message >> 8);

	out_buffer[0] = COMMAND_CONFIGURE;
	out_buffer[1] = message_0;
	out_buffer[2] = message_1;

	out_buffer_length = 3;

	return true;
}

void Veml7700::get_command_white_channel_output(uint8_t* out_buffer, uint8_t& out_buffer_length) const {
	out_buffer[0] = COMMAND_WHITE_CHANNEL;
	out_buffer[1] = _address_7bit << 1 | MODE_WRITE;
	out_buffer_length = 2;
}

void Veml7700::cache_raw_results_white_channel(const uint8_t* results) {
	uint16_t raw = results[1] << 8 | results[0];
	_lux_white_channel = static_cast<uint32_t>(raw * 0.1152f);  // TODO: this is hard-coded for a single config
}

uint32_t Veml7700::get_white_channel_lux() const {
	return _lux_white_channel;
}
