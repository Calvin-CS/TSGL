/*
 * Timer.h provides a simple timer for sleeping and keeping track of iterations
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 7/8/2014
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>        // For timing
#include <thread>        // For sleeping

typedef std::chrono::high_resolution_clock highResClock;

class Timer {
 private:
    unsigned int lastRep;
    std::chrono::duration<double> period_;
    highResClock::time_point startTime, lastTime;
 public:
    Timer(double period) {
        reset(period);
    }

    virtual ~Timer() {}

    // Get the number of reps since start
    unsigned int getReps() {
        return (highResClock::now() - startTime) / period_;
    }

    // Get the time since start
    double getTime() {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(highResClock::now() - startTime).count()
            / 1000000000.0;
    }

    // Check if the timer has elapsed past the point when it last past the period
    bool pastPeriod() {
        if (lastRep < getReps()) {
            lastRep = getReps();
            return true;
        } else
            return false;
    }

    // Changes the period to the given interval
    void reset(double period) {
        startTime = lastTime = highResClock::now();
        period_ = std::chrono::duration<double>(period);
        lastRep = 0;
    }

    // Sleep the thread until the period has passed
    void sleep() {
//        if (lastTime + period_ < highResClock::now()) std::cout << "no sleep" << std::endl;
        std::this_thread::sleep_until(lastTime + period_);
        lastTime = highResClock::now();
    }

    static void threadSleepFor(double duration) {
        std::this_thread::sleep_for(std::chrono::duration<double>(duration));
    }
};

#endif /* TIMER_H_ */
