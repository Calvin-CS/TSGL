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

    /*!
     * \brief Default Timer constructor method.
     * \details This is the default constructor for the Timer class.
     *   \param period Time in seconds specifying the maximum amount of time to sleep.
     * \return A new Timer with the specified period.
     */
    Timer(double period);

    /*!
     * \brief Timer destructor method.
     * \details This is the destructor for the Timer class.
     * \details Frees up memory that was allocated to a Timer instance.
     */
    virtual ~Timer();

    /*!
     * \brief Gets the number of repetitions since starting the timer.
     * \return The number of times the <code>period</code> has elapsed since the Timer has been started.
     */
    unsigned int getReps() const;

    /*!
     * \brief Gets the elapsed time since starting the timer
     * \return The time in seconds since starting the Timer.
     */
    double getTime() const;

    /*!
     * \brief Get the elapsed time between sleeps
     * \details This function returns the time in seconds between returning from the last two calls to sleep().
     *   This should be the same as <code>period</code> in cases where the Timer is allowed to sleep. If the
     *   Timer has yet to sleep twice, getTimeBetweenSleeps() is undefined, but will most likely be less than 0.
     * \return The time in seconds between the last two sleeps.
     */
    double getTimeBetweenSleeps() const;

    /*!
     * \brief Check if the Timer's period has elapsed.
     * \details This function returns whether the period of the Timer has elapsed since the last time
     *   the function was called.
     * \return True if at least <code>period</code> seconds have elapsed since the last call, false otherwise.
     */
    bool pastPeriod();

    /*!
     * \brief Reset the Timer.
     * \details This function resets the starting time, repetitions, and period of the timer.
     *   \param period The new period for the Timer. Setting this less than or equal to 0 will keep
     *     the current period.
     */
    void reset(double period = 0);

    /*!
     * \brief Sleeps the Timer's current thread until its period elapses.
     * \details This function tells the currently executing thread to sleep until the rest of the Timer
     *   instance's remaining period expires.
     * \details If the Timer's period has elapsed since last call, the thread will continue execution
     *   normally until the next call to sleep().
     *   \param update Whether to update the timer's last_rep status or not.
     * \note This function does not guarantee the thread will resume immediately after the time expires.
     *   Depending on your OS, the thread may sleep for longer.
     * \see getTimeBetweenSleeps(), to get the actual elapsed time between sleeps.
     */
    void sleep(bool update = true);

    /*!
     * \brief Sleeps the current thread for the specified duration.
     * \details This function tells the currently executing thread to sleep for <code>duration</code> seconds.
     * \note This function does not guarantee the thread will resume immediately after <code>duration</code> expires.
     *   Depending on your OS, the thread may sleep for longer.
     */
    static void threadSleepFor(double duration);    // Sleep the thread for a specified duration
};

}

#endif /* TIMER_H_ */
