#pragma once

/** @brief Simple class that starts at a configured number and increments on each call
 *         by the configured step.
 */
class DelayIncrementer {
public:
	/** @brief Configuration required at construction.
	 * @param starting_point is the number to start at and revert to after max_point is reached.
	 * @param step_amount is the number to be added on each call to next_increment.
	 * @param max_point is the max number to return before reverting back to starting_point.
	 */
	DelayIncrementer(int starting_point, int step_amount, int max_point);

	/** @return Adds the step_amount to the current value and returns. */
	int next_increment();

private:
	int _starting_point = 0;  ///< user configured lowest value
	int _max_point = 0;       ///< user configured largest value (before rolling over)
	int _step_amount = 0;     ///< user configured amount to add each increment
	int _current = 0;         ///< calculated incremented value
};
