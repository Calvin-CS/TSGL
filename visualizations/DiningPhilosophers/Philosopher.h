/*!
 * \class Philosopher
 * \brief Object representing a philosopher in the Dining Philosophers' problem
 * \details The Philosopher class contains variables and methods necessary for
 *   representing a philosopher at a table. Each Philosopher may acquire or release
 *   the fork to his left or to his right (or both), with his state changing
 *   accordingly.
 */
#ifndef PHILOSOPHER_H_
#define PHILOSOPHER_H_

#include <tsgl.h>
#include "Fork.h"
#include "enums.h" //TODO: make cleaner, move enums?

using namespace tsgl;

class Philosopher {
private:
  PhilState myState;
  PhilAction myAction;
  int id, myLeft, myRight, meals;
  Circle * myCircle;
public:
  Philosopher();
  ~Philosopher();
  void draw(Canvas& can, int x, int y);
  bool acquire(Fork& f);
  bool release(Fork& f);
  void think();
  int getMeals() { return meals; }
  void eat() { ++meals; myState = thinking; myAction = doNothing; }
  PhilState state() { return myState; }
  void setState(PhilState p) { myState = p; }
  PhilAction action() { return myAction; }
  void setAction(PhilAction a) { myAction = a; }
  int getId() { return id; }
  void setId(int i, int nphil) {id = myLeft = i; myRight = (id+nphil-1)%nphil; }
};

#endif /* PHILOSOPHER_H_ */
