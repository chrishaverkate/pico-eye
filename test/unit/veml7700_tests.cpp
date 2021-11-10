
#include <gtest/gtest.h>
#include <memory>

#include <pico-eye/veml7700.h>

class Veml7700Tests : public ::testing::Test {
protected:

	Veml7700 _sensor;
	uint8_t _buffer[10]{};
	uint8_t _buffer_length = 0;
};

TEST_F(Veml7700Tests, get_command_config) {
	const uint8_t expected_buff_0 = Veml7700::COMMAND_CONFIGURE;

	bool valid_config = _sensor.get_command_config(_buffer, _buffer_length);

	ASSERT_TRUE(valid_config);
	ASSERT_EQ(3, _buffer_length);
	ASSERT_EQ(expected_buff_0, _buffer[0]);
	ASSERT_EQ(0, _buffer[1]);
	ASSERT_EQ(0x2, _buffer[2]);
}

TEST_F(Veml7700Tests, get_command_white_channel_output) {
	const uint8_t expected_buff_0 = Veml7700::COMMAND_WHITE_CHANNEL;

	_sensor.get_command_white_channel_output(_buffer, _buffer_length);

	ASSERT_EQ(2, _buffer_length);
	ASSERT_EQ(expected_buff_0, _buffer[0]);
	ASSERT_EQ(0x21, _buffer[1]);
}

TEST_F(Veml7700Tests, cache_and_get_white_channel_lux) {
//	d9 0  -> 25.00 lux
	uint8_t buffer[2] = {0xd9, 0x00};  // 25 lux at 50ms with 1x gain
	_sensor.cache_raw_results_white_channel(buffer);

	EXPECT_NEAR(25.00, _sensor.get_white_channel_lux(), 1);
}

/**
 * tests
 * C) cache white light reading
 * D) get white light lux
 */




