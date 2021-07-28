/*
 * Timer.h provides a simple timer for sleeping and keeping track of iterations.
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

namespace tsgl {

/*! \class Timer
 *  \brief A class for various timing operations.
 *  \details Timer provides a simple timer for timing, sleeping threads, and keeping track of the
 *    current rendering frame.
 */
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

    unsigned int getReps() const;

    double getTime() const;

    double getTimeBetweenSleeps() const;

    bool pastPeriod();

    void reset(double period = 0);

    void sleep(bool update = true);

    static void threadSleepFor(double duration);    // Sleep the thread for a specified duration
};

}

#endif /* TIMER_H_ */
