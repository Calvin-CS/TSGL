#include "Timer.h"

namespace tsgl {

/*!
 * \brief Default Timer constructor method.
 * \details This is the default constructor for the Timer class.
 *   \param period Time in seconds specifying the maximum amount of time to sleep.
 * \return A new Timer with the specified period.
 */
Timer::Timer(double period) {
    reset(period);
}

/*!
 * \brief Timer destructor method.
 * \details This is the destructor for the Timer class.
 * \details Frees up memory that was allocated to a Timer instance.
 */
Timer::~Timer() {
}

// Get the number of reps since start
/*!
 * \brief Gets the number of repetitions since starting the timer.
 * \return The number of times the <code>period</period> has elapsed since the Timer has been started.
 */
unsigned int Timer::getReps() const {
    return std::chrono::duration_cast<duration_d>(highResClock::now() - start_time).count() / period_.count();
}

// Get the time since start
/*!
 * \brief Gets the elapsed time since starting the timer
 * \return The time in seconds since starting the Timer.
 */
double Timer::getTime() const {
    return std::chrono::duration_cast<duration_d>(highResClock::now() - start_time).count();
}

// Get the time between the two last sleeps
/*!
 * \brief Get the elapsed time between sleeps
 * \details This function returns the time in seconds between returning from the last two calls to sleep().
 *   This should be the same as <code>period</code> in cases where the Timer is allowed to sleep. If the
 *   Timer has yet to sleep twice, getTimeBetweenSleeps() is undefined, but will most likely be less than 0.
 * \return The time in seconds between the last two sleeps.
 */
double Timer::getTimeBetweenSleeps() const {
    return time_between_sleeps;
}

// Check if the timer has elapsed past the point when it last past the period
/*!
 * \brief Check if the Timer's period has elapsed.
 * \details This function returns whether the period of the Timer has elapsed since the last time
 *   the function was called.
 * \return True if at least <code>period</code> seconds have elapsed since the last call, false otherwise.
 */
bool Timer::pastPeriod() {
    if (last_rep < getReps()) {
        last_rep = getReps();
        return true;
    }
    return false;
}

// Changes the period to the given interval
/*!
 * \brief Reset the Timer.
 * \details This function resets the starting time, repetitions, and period of the timer.
 *   \param period The new period for the Timer. Setting this less than or equal to 0 will keep
 *     the current period.
 */
void Timer::reset(double period) {
    start_time = last_time = highResClock::now();
    if (period > 0) period_ = duration_d(period);
    last_rep = 0;
}

// Sleep the thread until the period has passed
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
void Timer::sleep(bool update) {
    mutexLock sleepLock(sleep_);
    timepoint_d sleep_time = last_time;
    while (sleep_time < highResClock::now()) {
      sleep_time = sleep_time + period_;
    }
    sleepLock.unlock();

    if (update)
      last_time = sleep_time;

    long double nano = std::chrono::duration_cast<duration_d>(sleep_time - highResClock::now()).count();
    std::this_thread::sleep_for(std::chrono::nanoseconds((long long) (nano * 1000000000)));

    time_between_sleeps = std::chrono::duration_cast<duration_d>(highResClock::now() - sleep_time).count()
        + period_.count();
}

// Sleep the thread for a specified duration
/*!
 * \brief Sleeps the current thread for the specified duration.
 * \details This function tells the currently executing thread to sleep for <code>duration</code> seconds.
 * \note This function does not guarantee the thread will resume immediately after <code>duration</code> expires.
 *   Depending on your OS, the thread may sleep for longer.
 */
void Timer::threadSleepFor(double duration) {
    std::this_thread::sleep_for(std::chrono::nanoseconds((long long) (duration * 1000000000)));
}

}
