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
#include <vector>
#include <mutex>
#include "Fork.h"
#include "philEnums.h"
// #include <omp.h> // saferand()
// #include <random> // saferand()

using namespace tsgl;

class Philosopher {
private:
  PhilState myState;
  PhilAction myAction;
  int id, myLeft, myRight;
  Circle * myCircle;
  std::vector<RegularPolygon*> meals;
  std::mutex mealLock;
public:
  Philosopher();
  ~Philosopher();
  void draw(Canvas& can, int x, int y);
  void refreshColor();
  bool acquire(Fork& f);
  bool release(Fork& f);
  void think();
  void eat() { myState = thinking; myAction = doNothing;}
  void addMeal(Canvas& can, RegularPolygon * shape);
  void clearMeals(Canvas& can);
  void setState(PhilState s) { myState = s; }
  void setAction(PhilAction a) { myAction = a; }
  void setId(int i, int nphil) {id = myLeft = i; myRight = (id+nphil-1)%nphil; }

  //Accessors
  int getMeals() { return meals.size(); }
  PhilState state() { return myState; }
  PhilAction action() { return myAction; }
  int getId() { return id; }
  bool hasCircle() { return myCircle; }
};

#endif /* PHILOSOPHER_H_ */
