#include "Philosopher.h"

Philosopher::Philosopher() {
  setId(0,1);
  myState = hasNone;
  myAction = doNothing;
  myCircle = NULL;
}

Philosopher::~Philosopher() {
  delete myCircle; myCircle = NULL;
  for(unsigned i = 0; i < meals.size(); i++) {
    delete meals[i];
  }
}

/**
 * Adds Philosopher to Canvas or refreshes its color.
 */
void Philosopher::draw(Canvas& can, int x, int y) {
  const int SIZE = 32;
  if( myCircle == NULL ) {
    myCircle = new Circle(x,y,SIZE,RED);
    can.add(myCircle);
  }
}

/**
 * Updates the Philosopher's color based on its state
 */
void Philosopher::refreshColor() {
  ColorFloat c;
  switch(myState) {
    case hasNone:  c=RED;    break;
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
void Philosopher::addMeal(Canvas& can, Circle * c) {
  can.add(c);
  meals.push_back(c);
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
  if(rand()%3 == 0) { // 1/3 probability to go to hungry state
    setState(hasNone);
    setAction(doNothing);
  }
}
