/*!
 * \class Table3D
 * \brief Object managing the forks and philosophers in the Dining Philosophers' problem.
 * \details The Table3D class keeps track of the forks and philosophers in the Dining
 *   Philosophers' problem; it additionally manages the actions of the philosophers.
 * \details Each step of the problem is broken up into two phases. In the checking phase,
 *   the philosophers look at the 3D table around them and, without communicating with the
 *   other philosophers, determine an action to take based on their state and the states
 *   of their adjacent forks.
 * \details In the action phase, each philosopher attempts to execute the action previously
 *   determined in the checking phase. If unsuccessful, the philosopher does nothing;
 *   otherwise, the philosopher's state changes depending on the action taken.
 */

#ifndef TABLE3D_H_
#define TABLE3D_H_

#include <tsgl.h>
#include "Philosopher3D.h"

using namespace tsgl;

class Table3D {
private:
  int numPhils;
  PhilMethod myMethod;
  std::wstring methodString;
  Canvas *myCan, *myCan2;
  Philosopher3D *phils;
  Fork3D *forks;
  Cylinder * myTable;
  Image ** spaghettis;
public:
  Table3D(Canvas& can, int p, PhilMethod m);

  ~Table3D();

  void forfeitWhenBlockedMethod(int id);

  void waitWhenBlockedMethod(int id);

  void nFrameReleaseMethod(int id);

  void hierarchyMethod(int id);

  void oddEvenMethod(int id);

  void checkStep();

  void actStep();

  void drawStep();
};

#endif /* TABLE3D_H_ */