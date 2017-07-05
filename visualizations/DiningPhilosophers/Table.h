/*!
 * \class Table
 * \brief Object managing the forks and philosophers in the Dining Philosophers' problem.
 * \details The Table class keeps track of the forks and philosophers in the Dining
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

class Table {
private:
  int tabX, tabY, numPhils;
  PhilMethod myMethod;
  Canvas *myCan, *myCan2;
  Philosopher *phils;
  Fork *forks;
  Circle *myCircle;
public:
  Table(Canvas& can, int p, PhilMethod m);
  ~Table();

  void forfeitWhenBlockedMethod(int id);
  void waitWhenBlockedMethod(int id);
  void nFrameReleaseMethod(int id);
  void hieararchyMethod(int id);
  void oddEvenMethod(int id);

  void checkStep();
  void actStep();
  void drawStep();

};

#endif /* TABLE_H_ */
