
#include <gtest/gtest.h>
#include <memory>

#include <project-template/delay_incrementer.h>

using std::unique_ptr;
using std::make_unique;

class DelayIncrementerTests : public ::testing::Test {
protected:
	const int _number_of_steps = 3;
	const int _start = 1;
	const int _step = 2;
	const int _max = _start + (_number_of_steps * _step);

	unique_ptr<DelayIncrementer> delayer;

	void SetUp() override {
		delayer = make_unique<DelayIncrementer>(_start, _step, _max);
	}
};

TEST_F(DelayIncrementerTests, first_step_starting_plus_step) {
	ASSERT_EQ(_start + _step, delayer->next_increment());
}

TEST_F(DelayIncrementerTests, land_on_max) {
	for(int i(0); i < _number_of_steps - 1; ++i) {
		delayer->next_increment();
	}

	ASSERT_EQ(_max, delayer->next_increment());
}

TEST_F(DelayIncrementerTests, rollover_to_starting) {
	for(int i(0); i < _number_of_steps; ++i) {
		delayer->next_increment();
	}

	ASSERT_EQ(_start, delayer->next_increment());
}

