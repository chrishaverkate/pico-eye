
#include "project-template/delay_incrementer.h"

DelayIncrementer::DelayIncrementer(int starting_point, int step_amount, int max_point)
    : _starting_point(starting_point)
    , _step_amount(step_amount)
    , _max_point(max_point)
    , _current(starting_point) {
}

int DelayIncrementer::next_increment() {
	_current += _step_amount;

	if (_current > _max_point) {
		_current = _starting_point;
	}

	return _current;
}
