#pragma once
#include <stdint.h>

/** @brief A stateful encapsulation of the TCS3471 specific commands and data parsing.
 * Provides useful conversions between type-specific configurable parameters and raw
 * buffers for sending over i2c.
 *
 * Also provides calculation for response data to useful units, such as lux and more.
 */
class Tcs3471 {
public:
	static const uint8_t COMMAND_MASK = 0x01 << 7 | 0x01 << 6; // Selecting command reg and auto-increment to next

	static const uint8_t REG_ENABLE = 0x0;
	static const uint8_t REG_RGBC_TIMING = 0x01;
	static const uint8_t REG_WAIT_TIME = 0x03;
	static const uint8_t REG_INT_THRESH_LOW_LOW = 0x04;   ///< low threshold, low byte
	static const uint8_t REG_INT_THRESH_LOW_HIGH = 0x05;  ///< low threshold, high byte
	static const uint8_t REG_INT_THRESH_HIGH_LOW = 0x06;  ///< high threshold, low byte
	static const uint8_t REG_INT_THRESH_HIGH_HIGH = 0x07; ///< high threshold, high byte

	static const uint8_t REG_INT_PERSISTENCE = 0x0C;
	static const uint8_t REG_CONFIGURATION = 0x0D;
	static const uint8_t REG_GAIN = 0x0F;

	static const uint8_t REG_ID = 0x12;
	static const uint8_t REG_STATUS = 0x13;

	static const uint8_t REG_DATA_LOW_CLEAR = 0x14;
	static const uint8_t REG_DATA_HIGH_CLEAR = 0x15;
	static const uint8_t REG_DATA_LOW_RED = 0x16;
	static const uint8_t REG_DATA_HIGH_RED = 0x17;
	static const uint8_t REG_DATA_LOW_GREEN = 0x18;
	static const uint8_t REG_DATA_HIGH_GREEN = 0x19;
	static const uint8_t REG_DATA_LOW_BLUE = 0x1A;
	static const uint8_t REG_DATA_HIGH_BLUE = 0x1B;

	// Enable register modes
	static const uint8_t ENABLE_MODE_WRITE = 0x01;

	/** @brief Fill a buffer for configuring a TCS3471 sensor
	 * @param[out] out_buffer to be filled
	 * @param[out] out_buffer_length number of bytes in buffer
	 * @return true if valid configuration TODO: add more options for configuring.
	 */
	bool get_command_config(uint8_t* out_buffer, uint8_t& out_buffer_length);

	/** @return the cached lux calculation. */
	float get_white_channel_lux() const;

	float get_red_percent() const;
	float get_green_percent() const;
	float get_blue_percent() const;
	float get_clear_percent() const;

	// TODO create function to take 27 bytes in as a raw buffer of all bytes from the sensor
	// In there will be the details of the configuration (IT, gain, etc) plus the RGBC data
	void cache_raw_buffers(const uint8_t* results);

	uint16_t get_max_count() const;

	uint16_t get_saturation_count() const;

	int calculate_cct();
	int cct(int R, int G, int B);

private:
	uint8_t _address_7bit = 0x29;  ///< Default i2c address in 7-bit addressing
	uint8_t _config_buffer[16];
	uint8_t _raw_results[28];
};
