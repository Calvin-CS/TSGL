#include "Timer.h"

Timer::Timer(double period){
  reset(period);
}

Timer::~Timer(){}

unsigned int Timer::getReps() const {
  return std::chrono
    ::duration_cast<duration_d>
    (highResClock::now() - start_time).count()/period_.count();
}
    
double Timer::getTime() const {
  return std::chrono
    ::duration_cast<duration_d>
    (highResClock::now() - start_time).count();
}

double Timer::getTimeBetweenSleeps() const {
  return time_between_sleeps;
}

bool Timer::pastPeriod() {
  if(last_rep < getReps()){
    last_rep = getReps();
    return true;
  }
  return false;
}

void Timer::reset(double period){
  start_time = last_time = highResClock::now();
  if(period > 0) period_ = duration_d(period);
  last_rep = 0;
}

void Timer::sleep(bool update){
  mutexLock sleepLock(sleep_);
  timepoint_d sleep_time = last_time;
  while(sleep_time < highResClock::now()){
    sleep_time = sleep_time + period_;
  }
  sleepLock.unlock();

  if(update)
    last_time = sleep_time;

  long double nano = std::chrono
    ::duration_cast<duration_d>(sleep_time - highResClock::now()).count();
  std::this_thread::sleep_for
    (std::chrono::nanoseconds((long long) (nano * 1000000000)));

  time_between_sleeps = std::chrono
    ::duration_cast<duration_d>(highResClock::now() - sleep_time).count()
    +period_.count();
}

void Timer::threadSleepFor(double duration) {
  std::this_thread::sleep_for
    (std::chrono::nanoseconds((long long) (duration * 1000000000)));
}
