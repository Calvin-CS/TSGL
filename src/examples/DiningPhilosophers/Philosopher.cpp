#include "Philosopher.h"

/*!
 *  \brief Explicitly constructs a new Philosopher.
 *  \details Explicit constructor for a new Philosopher object.
 */
Philosopher::Philosopher() {
  setId(0,1);
  myState = thinking;
  myAction = doNothing;
  myCircle = NULL;
  numMeals = 0;
}

Philosopher::~Philosopher() {
  delete myCircle;
  for (RegularPolygon * meal : meals)
  {
    delete meal;
  } 
  meals.clear();
}

/**
 * Adds Philosopher to Canvas or refreshes its color.
 */
void Philosopher::draw(Canvas& can, float x, float y) {
  const float SIZE = 45;
  if( !myCircle) {
    myCircle = new Circle(x,y,0,SIZE,0,0,0,ColorFloat(1,0,0,1));
    can.add(myCircle);
  }
}

/**
 * Updates the Philosopher's color based on its state
 */
void Philosopher::refreshColor() {
  ColorFloat c;
  switch(myState) {
    case hasNone:  c=RED;   break;
    case hasRight: c=ORANGE; break;
    case hasLeft:  c=PURPLE; break;
    case hasBoth:  c=GREEN;  break;
    case isFull:   c=BLUE;   break;
    case thinking: c=BLUE;   break;
  }
  myCircle->setColor(c);
}

/**
 * Adds a meal representation to meals and the Canvas
 */
void Philosopher::addMeal(float x, float y, float z) {
  numMeals++;
  meals.push_back(new RegularPolygon(x,y,z,3,3,0,0,0,ColorFloat(0.5,0.3,0,1)));
}

RegularPolygon * Philosopher::getLastMeal() {
  return meals.back();
}

/**
 * Picks up a fork specified by its reference
 */
bool Philosopher::acquire(Fork& f) {
  if (f.user >= 0)
    return false;
  if (f.id == myLeft) {
    if (myState == hasNone)
      myState = hasLeft;
    else if (myState == hasRight)
      myState = hasBoth;
    else
      return false;
    f.user = id;
    return true;
  }
  if (f.id == myRight) {
    if (myState == hasNone)
      myState = hasRight;
    else if (myState == hasLeft)
      myState = hasBoth;
    else
      return false;
    f.user = id;
    return true;
  }
  return false;
}

/**
 * Releases a fork specified by its reference
 */
bool Philosopher::release(Fork& f) {
  if (f.user != id)
    return false;
  if (myState != isFull)
    myState = (myState == ((f.id == myLeft) ? hasLeft : hasRight)) ? hasNone : isFull;
  f.user = -1;
  return true;
}

/**
 * Thinks and switches to hungry state if a random number is a multiple of 3.
 */
void Philosopher::think() {
  if(saferand(1,9999)%3 == 0) { // 1/3 probability to go to hungry state
    setState(hasNone);
    setAction(doNothing);
  }
}
