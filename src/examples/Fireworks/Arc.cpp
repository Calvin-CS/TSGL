/*
 * Arc.cpp
 */ 

#include "Arc.h"

const float HFLOAT = 6.0f/255.0f;

/*!
 * \brief Explicit constructor for an Arc.
 * \details Explicitly constructs an Arc.
 * \param can The Canvas to which the Arc is to be drawn.
 */
Arc::Arc(Canvas& can) {
  f = NULL;
  myLife = 0;
  myCan = &can;
  myBackground = myCan->getBackground();
  myX = saferand(-myCan->getWindowWidth()/2,myCan->getWindowWidth()/2);
  myY = saferand(-myCan->getWindowHeight()/2,myCan->getWindowHeight()/2);
  myAngle = (randfloat(32000)) * 2.0f*PI;
  myRad = 20 + saferand(0,179);
  computeStepSize();
  myColor = ColorHSV(0.0f,1.0f,1.0f,1.0f);
}

/*!
 * \brief Explicit constructor for an Arc.
 * \details Explicitly constructs an Arc.
 * \param can The Canvas to which the dot is to be drawn.
 * \param x The x coordinate of the dot emission point.
 * \param y The y coordinate of the dot emission point.
 * \param rad The radians of the Arc.
 * \param angle The starting angle of the Arc.
 */
Arc::Arc(Canvas* can, int x, int y, int rad, float angle) {
  myCan = can;
  myX = x; myY = y;
  myAngle = angle; myRad = rad;
  computeStepSize();
}

/*!
 * \brief Accessor for if the Arc is out of the Canvas window.
 * \details Returns if myX and myY are between 0 and the Canvas' width and height respectively.
 */
bool Arc::outOfBounds() {
  return (myX < -myCan->getWindowWidth()/2 || myY < -myCan->getWindowHeight()/2 || myX > myCan->getWindowWidth()/2 || myY > myCan->getWindowHeight()/2);
}

/*!
 * \brief Accesssor if the Canvas is black at the Arc's pixel.
 */
bool Arc::onBlackPixel() {
  const int LET = 14;
  ColorInt col = myCan->getBackground()->getPixel(myX,myY);
  return !(col.R<LET && col.G<LET && col.B<LET);
}

/*!
 * \brief Computes the Arc's steps size based on myRad and myAngle.
 */
void Arc::computeStepSize() {
  myStepSize = 1.0f/myRad;
  if (saferand(0,1) == 0) {
    myStepSize = -myStepSize;
    myAngle = -myAngle;
  }
  myColor.H = HFLOAT*(++myLife % 255);
}

/*!
 * \brief Moves the Arc to a different location.
 */
void Arc::relocate() {
  myRad = 20 + rand() % 180;
  myAngle = ((rand() % 32000) / 32000.0f) * 2.0f*PI;
  while (outOfBounds() || onBlackPixel()) {
    myX = saferand(-myCan->getWindowWidth()/2,myCan->getWindowWidth()/2);
    myY = saferand(-myCan->getWindowHeight()/2,myCan->getWindowHeight()/2);
  }
  computeStepSize();
}

/*!
 * \brief Makes the Arc take a step.
 */
void Arc::step() {
  if (f != NULL)
    f->step();
  if (saferand(0,99) < 2) {
    ++myRad;
    myStepSize = 1.0f/(myRad);
  }
  myAngle += myStepSize;
  myX += cos(myAngle);
  myY += sin(myAngle);
  if (outOfBounds() || onBlackPixel()) {
    if (f != NULL)
      delete f;
    f = new Firework(*myCan,myX,myY);
    relocate();
  }
  myBackground->drawPixel(myX,myY,myColor);
}

Arc::~Arc() {
  delete f;
}