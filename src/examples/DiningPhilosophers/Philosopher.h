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
#include "Fork.h"
#include "philEnums.h"

class Philosopher {
private:
  PhilState myState;
  PhilAction myAction;
  int id, myLeft, myRight;
  unsigned int numMeals;
  Circle *myCircle;
public:
  Philosopher();
  ~Philosopher();
  void draw(Canvas& can, float x, float y);
  void refreshColor();
  void addMeal();
  bool acquire(Fork& f);
  bool release(Fork& f);
  void think();

  // Mutators
  /*!
   * \brief Resets the Philosopher to thinking after he eats.
   */
  void eat() { myState = thinking; myAction = doNothing;}
  /*!
   * \brief Mutator for myState
   *  \param s PhilState to set myState to.
   */
  void setState(PhilState s) { myState = s; }
  /*!
   * \brief Mutator for myAction
   *  \param s PhilAction to set myAction to.
   */
  void setAction(PhilAction a) { myAction = a; }
  /*!
   * \brief Mutator for id
   *  \param i Which philosopher id to mutate
   *  \param nphil Total number of philosophers.
   */
  void setId(int i, int nphil) {id = myLeft = i; myRight = (id+nphil-1)%nphil; }

  //Accessors
  /**
   *  Accessor for number of meals Philosopher has consumed.
   */
  int getMeals() { return numMeals; }
  /**
   *  Accessor for Philosopher's state.
   */
  PhilState state() { return myState; }
  /**
   *  Accessor for Philosopher's action.
   */
  PhilAction action() { return myAction; }
  /**
   *  Accessor for Philosopher's id.
   */
  int getId() { return id; }
  /**
   *  Accessor for Philosopher's circle.
   */
  bool hasCircle() { return myCircle; }
};

#endif /* PHILOSOPHER_H_ */