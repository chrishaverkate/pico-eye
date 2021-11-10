
#include <gtest/gtest.h>
#include <memory>

#include <pico-eye/tcs3471.h>

class Tcs3471Tests : public ::testing::Test {
protected:

	Tcs3471 _sensor;
	uint8_t _buffer[28]{};
};

TEST_F(Tcs3471Tests, calc_percent_red) {
	uint16_t red = 0x02 << 8 | 0x01;
	_buffer[Tcs3471::REG_DATA_HIGH_RED] = red >> 8;
	_buffer[Tcs3471::REG_DATA_LOW_RED] = red | 0xFF;

	_sensor.cache_raw_buffers(_buffer);

	float actual_red_percent = _sensor.get_red_percent();
	printf("%.2f\n", actual_red_percent);
}

TEST_F(Tcs3471Tests, calculate_cct) {
	int R = 231;
	int G = 260;
	int B = 95;

	ASSERT_EQ(3121, _sensor.cct(R, G, B));
}




