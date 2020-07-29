/*
 * Dot.cpp
 */  

#include "Dot.h"

/*!
 * \brief Explicit constructor for a Dot.
 * \details Draws a Dot to a Canvas parameter
 *    \param can Canvas to which the Dot will be drawn.
 *    \param x The x-coordinate of the Dot.
 *    \param y The y-coordinate of the Dot.
 *    \param s The Dot's movement speed.
 *    \param f The Dot's friction.
 * 
 */
Dot::Dot(Canvas& can, float x, float y, float s, float d, float f) {
  myCan = &can;
  myBackground = myCan->getBackground();
  dead = false;
  myX = x; myY = y; mySpeed = s;
  myDir = d; myFric = f;
}

/*!
 * \brief Makes the Dot take a step.\
 * \details Updates the Dot's x and y based on its speed and friction
 */
void Dot::step() {
  myX += mySpeed*cos(myDir);
  myY += mySpeed*sin(myDir);
  mySpeed *= myFric;
  if (!dead && abs(myX) < myCan->getWindowWidth() / 2 && abs(myY) < myCan->getWindowHeight() / 2)
    myBackground->drawPixel(myX,myY,ColorInt(255,255,255,255));
  if (mySpeed < 0.5f)
    dead = true;
}

/*!
 * \brief Accessor for the Dot's death status
 * \details Returns the Dot's dead variable.
 * \return dead The Dot's variable that says whether it's dead.
 */
bool Dot::isDead() { 
  return dead; 
}