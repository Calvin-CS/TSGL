/*
 * Timer.h provides a simple timer for sleeping and keeping track of iterations
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 7/8/2014
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>        // For timing
#include <mutex>         // Needed for locking for thread-safety
#include <thread>        // For sleeping

typedef std::chrono::high_resolution_clock highResClock;
typedef std::chrono::duration<double> duration_d;
typedef std::chrono::time_point<highResClock, duration_d> timepoint_d;

class Timer {
 private:
    typedef std::unique_lock<std::mutex> mutexLock;

    unsigned int lastRep;
    std::chrono::duration<double> period_;
    timepoint_d startTime, lastTime;
    std::mutex sleep_;
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
        period_ = duration_d(period);
        lastRep = 0;
    }

    // Sleep the thread until the period has passed
    void sleep() {
        mutexLock sleepLock(sleep_);
        if (lastTime < highResClock::now())
            lastTime = lastTime + period_;
        sleepLock.unlock();
//        if (lastTime <= highResClock::now()) std::cout << "no sleep" << std::endl;
        std::this_thread::sleep_until(lastTime);
    }

    // Sleep the thread for a specified duration
    static void threadSleepFor(double duration) {
        std::this_thread::sleep_for(duration_d(duration));
    }
};

#endif /* TIMER_H_ */
