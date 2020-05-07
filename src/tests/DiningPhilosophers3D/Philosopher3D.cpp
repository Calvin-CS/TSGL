#include "Philosopher3D.h"

/*!
 *  \brief Explicitly constructs a new Philosopher3D.
 *  \details Explicit constructor for a new Philosopher3D object.
 */
Philosopher3D::Philosopher3D() {
  setId(0,1);
  myState = hasNone;
  myAction = doNothing;
  myCylinder = NULL;
  myCone = NULL;
  numMeals = 0;
}

Philosopher3D::~Philosopher3D() {
  delete myCylinder;
  delete myCone;
}

/**
 * Adds Philosopher3D to Canvas or refreshes its color.
 */
void Philosopher3D::draw(Canvas& can, float x, float y) {
  const float SIZE = .5;
  if( !myCylinder) {
    myCylinder = new Cylinder(x,y,-1,SIZE*4,SIZE,0,0,90,ColorGLfloat(1,0,0,1));
    can.add(myCylinder);
  }
  if( !myCone && myCylinder) {
    myCone = new Cone(x,y+SIZE*3,-1,SIZE*2.25,SIZE*1.25,0,0,90,ColorGLfloat(0.7,0,0,1));
    myCone->setRotationPoint(myCylinder->getCenterX(), myCylinder->getCenterY(), myCylinder->getCenterZ());
    can.add(myCone);
  }
}

/**
 * Updates the Philosopher3D's color based on its state
 */
void Philosopher3D::refreshColor() {
  ColorGLfloat c;
  switch(myState) {
    case hasNone:  c=ColorGLfloat(1,0,0,1);    break;
    case hasRight: c=ColorGLfloat(1,0.65,0,1); break;
    case hasLeft:  c=ColorGLfloat(0.75, 0.0, 0.75, 1.0); break;
    case hasBoth:  c=ColorGLfloat(0, 1.0, 0, 1.0);  break;
    case isFull:   c=ColorGLfloat(0,0,1, 1.0);   break;
    case thinking: c=ColorGLfloat(0,0,1, 1.0);   break;
  }
  myCylinder->setColor(c);
  myCone->setColor(ColorGLfloat(c.R*.7,c.G*.7,c.B*.7,c.A));
}

/**
 * Adds a meal representation to meals and the Canvas
 */
void Philosopher3D::addMeal() {
  numMeals++;
}

/**
 * Picks up a fork specified by its reference
 */
bool Philosopher3D::acquire(Fork3D& f) {
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
bool Philosopher3D::release(Fork3D& f) {
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
void Philosopher3D::think() {
  if(rand()%3 == 0) { // 1/3 probability to go to hungry state
    setState(hasNone);
    setAction(doNothing);
  }
}
