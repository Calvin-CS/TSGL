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
  myX = rand() % myCan->getWindowWidth();
  myY = rand() % myCan->getWindowHeight();
  myAngle = ((rand() % 32000) / 32000.0f) * 2.0f*PI;
  myRad = 20 + rand() % 180;
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
  return (myX < 0 || myY < 0 || myX > myCan->getWindowWidth() || myY > myCan->getWindowHeight());
}

/*!
 * \brief Accesssor if the Canvas is black at the Arc's pixel.
 */
bool Arc::onBlackPixel() {
  const int LET = 14;
  ColorInt col = myCan->getPoint(myX,myY);
  return !(col.R<LET && col.G<LET && col.B<LET);
}

/*!
 * \brief Computes the Arc's steps size based on myRad and myAngle.
 */
void Arc::computeStepSize() {
  myStepSize = 1.0f/myRad;
  if (rand() % 2 == 0) {
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
    myX = rand() % myCan->getWindowWidth();
    myY = rand() % myCan->getWindowHeight();
  }
  computeStepSize();
}

/*!
 * \brief Makes the Arc take a step.
 */
void Arc::step() {
  if (f != NULL)
    f->step();
  if (rand() % 100 < 2) {
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
  myCan->drawPoint(myX,myY,myColor);
}

Arc::~Arc() {
  delete f;
}