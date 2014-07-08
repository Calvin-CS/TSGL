/*
 * Timer.h provides a simple timer for sleeping and keeping track of iterations
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 7/8/2014
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>		// For timing
#include <thread>		// For sleeping

typedef std::chrono::high_resolution_clock highResClock;

class Timer {
private:
	highResClock::time_point startTime, lastTime;
	std::chrono::duration<double> period_;
	int lastRep;
public:
	Timer(double period) {
		startTime = lastTime = highResClock::now();
		period_ = std::chrono::duration<double>(period);
		lastRep = 0;
	}

	virtual ~Timer() {}

	// Get the number of reps since start
	int getReps() {
		return (highResClock::now() - startTime) / period_;
	}

	// Get the time since start
	double getTime() {
		return	std::chrono::duration_cast<std::chrono::nanoseconds>(
				highResClock::now() - startTime).count() / 1000000000.0;
	}

	bool pastPeriod() {
		if (lastRep < getReps()) {
			lastRep = getReps();
			return true;
		} else return false;
	}

	// Sleep the thread until the period has passed
	void sleep() {
		std::this_thread::sleep_until(lastTime + period_);
		lastTime = highResClock::now();
	}
};

#endif /* TIMER_H_ */
