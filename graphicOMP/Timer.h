/*
 * Timer.h provides a simple timer for sleeping and keeping track of iterations
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 6/20/2014
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
public:
	Timer(double period) {
		startTime = lastTime = highResClock::now();
		period_ = std::chrono::duration<double>(period);
	}

	virtual ~Timer() {}

	void sleep() {
		std::this_thread::sleep_until(lastTime + period_);
		lastTime = highResClock::now();
	}

	double getTime() { return	 std::chrono::duration_cast<std::chrono::nanoseconds>(
								 highResClock::now() - startTime).count() / 1000000000.0; }

	int getReps() { return (highResClock::now() - startTime) / period_; }
};

#endif /* TIMER_H_ */
