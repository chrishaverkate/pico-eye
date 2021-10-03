#pragma once

#include <stdint.h>

/** @brief A stateful encapsulation of the VEML7700 specific commands and data parsing.
 * Provides useful conversions between type-specific configurable parameters and raw
 * buffers for sending over i2c.
 *
 * Also provides calculation for response data to useful units, such as lux.
 */
class Veml7700 {
public:
	static const uint8_t COMMAND_CONFIGURE = 0x0;
	static const uint8_t COMMAND_WHITE_CHANNEL = 0x5;

	static const uint8_t MODE_WRITE = 0x01;


	/** @brief Fill a buffer for configuring a VEML7700 sensor
	 * @param[out] out_buffer to be filled
	 * @param[out] out_buffer_length number of bytes in buffer
	 * @return true if valid configuration TODO: add more options for configuring.
	 */
	bool get_command_config(uint8_t* out_buffer, uint8_t& out_buffer_length);

	/** @brief Fill a buffer for requesting the white channel data
	 * @param[out] out_buffer to be filled
	 * @param[out] out_buffer_length number of bytes in buffer
	 */
	void get_command_white_channel_output(uint8_t* out_buffer, uint8_t& out_buffer_length) const;

	/** @brief Given the raw bytes returned from the sensor, the result will be calculated and cached.
	 * @param[in] results raw bytes (should only be two per datasheet)
	 */
	void cache_raw_results_white_channel(const uint8_t* results);

	/** @return the cached lux calculation. */
	uint32_t get_white_channel_lux() const;

private:
	uint8_t _address_7bit = 0x10;  ///< Default i2c address of the VEML7700 sensor in 7-bit addressing

	uint32_t _lux_white_channel = 0;  ///< Cached calculation of the white channel lux (calculated in cache_raw...)

};
