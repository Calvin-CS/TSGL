/*!
 * \class Philosopher3D
 * \brief Object representing a 3D philosopher in the Dining Philosophers' problem
 * \details The Philosopher3D class contains variables and methods necessary for
 *   representing a philosopher at a table. Each Philosopher3D may acquire or release
 *   the fork to his left or to his right (or both), with his state changing
 *   accordingly.
 */

#ifndef PHILOSOPHER3D_H_
#define PHILOSOPHER3D_H_

#include <tsgl.h>
#include <vector>
#include "Fork3D.h"
#include "philEnums.h"

class Philosopher3D {
private:
  PhilState myState;
  PhilAction myAction;
  int id, myLeft, myRight;
  unsigned int numMeals;
  Cylinder *myCylinder;
  Cone * myCone;
public:
  Philosopher3D();
  ~Philosopher3D();
  void draw(Canvas& can, float x, float y);
  void refreshColor();
  void addMeal();
  bool acquire(Fork3D& f);
  bool release(Fork3D& f);
  void think();

  // Mutators
  /*!
   * \brief Resets the Philosopher3D to thinking after he eats.
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
   *  Accessor for number of meals Philosopher3D has consumed.
   */
  int getMeals() { return numMeals; }
  /**
   *  Accessor for Philosopher3D's state.
   */
  PhilState state() { return myState; }
  /**
   *  Accessor for Philosopher3D's action.
   */
  PhilAction action() { return myAction; }
  /**
   *  Accessor for Philosopher3D's id.
   */
  int getId() { return id; }
  /**
   *  Accessor for Philosopher3D's cylinder.
   */
  bool hasCylinder() { return myCylinder; }
};

#endif /* PHILOSOPHER3D_H_ */