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
#include <iostream>

typedef std::chrono::high_resolution_clock highResClock;
typedef std::chrono::duration<double> duration_d;
typedef std::chrono::time_point<highResClock, duration_d> timepoint_d;

class Timer {
 private:
    typedef std::unique_lock<std::mutex> mutexLock;

    unsigned int last_rep;
    duration_d period_;
    timepoint_d start_time, last_time;
    std::mutex sleep_;
    double time_between_sleeps;
 public:
    Timer(double period);
    virtual ~Timer();

    unsigned int getReps() const;                   // Get the number of reps since start
    double getTime() const;                         // Get the time since start
    double getTimeBetweenSleeps() const;            // Get the time between the two last sleeps
    bool pastPeriod();                              // Check if the timer has elapsed past the point when it last past the period
    void reset(double period = 0);                      // Changes the period to the given interval
    void sleep();                                   // Sleep the thread until the period has passed
    static void threadSleepFor(double duration);    // Sleep the thread for a specified duration
};

#endif /* TIMER_H_ */
