/*!
 * \class Simulation
 * \brief Object managing the forks and philosophers in the Dining Philosophers' problem.
 * \details The Simulation class keeps track of the forks and philosophers in the Dining
 *   Philosophers' problem; it additionally manages the actions of the philosophers.
 * \details Each step of the problem is broken up into two phases. In the checking phase,
 *   the philosophers look at the table around them and, without communicating with the
 *   other philosophers, determine an action to take based on their state and the states
 *   of their adjacent forks.
 * \details In the action phase, each philosopher attempts to execute the action previously
 *   determined in the checking phase. If unsuccessful, the philosopher does nothing;
 *   otherwise, the philosopher's state changes depending on the action taken.
 */
#ifndef TABLE_H_
#define TABLE_H_

#include <tsgl.h>
#include "Philosopher.h"
#include "Fork.h"
#include "philEnums.h"
using namespace tsgl;

class Simulation {
private:
  int tabX, tabY, numPhils, counter;
  PhilMethod myMethod;
  Canvas *myCan, *myCan2;
  Philosopher *phils;
  Fork *forks;
  Circle *myCircle;
public:
  Simulation(Canvas& can, int p, PhilMethod m);
  ~Simulation();

  void forfeitWhenBlockedMethod(int id);
  void waitWhenBlockedMethod(int id);
  void nCountReleaseMethod(int id);
  void hierarchyMethod(int id);
  void oddEvenMethod(int id);

  void checkStep();
  void actStep();
  void drawStep();

};

#endif /* TABLE_H_ */
