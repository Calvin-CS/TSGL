#include "Philosopher.h"

/*!
 *  \brief Explicitly constructs a new Philosopher.
 *  \details Explicit constructor for a new Philosopher object.
 */
Philosopher::Philosopher() {
  setId(0,1);
  myState = hasNone;
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
  const float SIZE = .45;
  if( !myCircle) {
    myCircle = new Circle(x,y,0,SIZE,0,0,0,ColorGLfloat(1,0,0,1));
    can.add(myCircle);
  }
}

/**
 * Updates the Philosopher's color based on its state
 */
void Philosopher::refreshColor() {
  ColorGLfloat c;
  switch(myState) {
    case hasNone:  c=ColorGLfloat(1,0,0,1);    break;
    case hasRight: c=ColorGLfloat(1,0.65,0,1); break;
    case hasLeft:  c=ColorGLfloat(0.75, 0.0, 0.75, 1.0); break;
    case hasBoth:  c=ColorGLfloat(0, 1.0, 0, 1.0);  break;
    case isFull:   c=ColorGLfloat(0,0,1, 1.0);   break;
    case thinking: c=ColorGLfloat(0,0,1, 1.0);   break;
  }
  myCircle->setColor(c);
}

/**
 * Adds a meal representation to meals and the Canvas
 */
void Philosopher::addMeal(float x, float y, float z) {
  numMeals++;
  meals.push_back(new RegularPolygon(x,y,z,.03,3,0,0,0,ColorGLfloat(0.5,0.3,0,1)));
  meals.back()->displayOutlineEdges(false);
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
  if(rand()%3 == 0) { // 1/3 probability to go to hungry state
    setState(hasNone);
    setAction(doNothing);
  }
}
