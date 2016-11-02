#ifndef _TIMER_
#define _TIMER_

#include <chrono>
#include <mutex>
#include <thread>
#include <iostream>

typedef std::chrono::high_resolution_clock highResClock;
typedef std::chrono::duration<double> duration_d;
typedef std::chrono::time_point<highResClock, duration_d> timepoint_d;

class Timer{
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

  static void threadSleepFor(double duration);
};

#endif
