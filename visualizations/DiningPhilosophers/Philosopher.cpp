#include "Philosopher.h"

Philosopher::Philosopher() {
  setId(0,1); meals = 0; myState = hasNone; myAction = doNothing; myCircle = NULL;
}
Philosopher::~Philosopher() {
  delete myCircle;
}
void Philosopher::draw(Canvas& can, int x, int y) {
  const int SIZE = 32;
  ColorFloat c;
  switch(myState) {
    case hasNone:  c=RED;    break;
    case hasRight: c=ORANGE; break;
    case hasLeft:  c=PURPLE; break;
    case hasBoth:  c=GREEN;  break;
    case isFull:   c=BLUE;   break;
    case thinking: c=BLUE;   break;
  }
  if( myCircle != NULL ) {
    can.remove(myCircle);
    delete myCircle;
  }
  myCircle = new Circle(x,y,SIZE,SIZE,c);
  can.add(myCircle);
}
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
bool Philosopher::release(Fork& f) {
  if (f.user != id)
    return false;
  if (myState != isFull)
    myState = (myState == ((f.id == myLeft) ? hasLeft : hasRight)) ? hasNone : isFull;
  f.user = -1;
  return true;
}
void Philosopher::think() {
  if(rand()%3 == 0) {
    setState(hasNone);
    setAction(doNothing);
  }
}
