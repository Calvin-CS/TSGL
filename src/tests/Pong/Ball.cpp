/*
 * Ball.cpp
 */

#include "Ball.h"


 /*!
  * \brief Explicitly construct the Ball object.
  * \details Explicit constructor for the Ball object.
  * \param can Reference to the Canvas that will have the Ball object.
  * \param speed Reference to the speed of the Ball object.
  * \return The constructed Ball object.
  */
Ball::Ball(Canvas& can, int & speed) {
  mySpeed = speed;
  myX = can.getWindowWidth() / 2-8;
  myY = can.getWindowHeight() / 2-8;
  do {
    myDir = randfloat(1000) * 2 * PI;
    myXX = mySpeed * cos(myDir);
    myYY = mySpeed * sin(myDir);
  } while(myXX > -4 && myXX < 4);
  myCircle = new Circle(myX, myY, 8, WHITE);
  can.add(myCircle);
}

 /*!
  * \brief Accessor for the Ball object's current x-coordinate.
  * \return myX The x-coordinate of the Ball object.
  */
float Ball::getX() const {
  return myX;
}

 /*!
  * \brief Accessor for the Ball object's current y-coordinate.
  * \return myY The y-coordinate of the Ball object.
  */
float Ball::getY() const {
  return myY;
}

 /*!
  * \brief Invert the Ball's direction.
  * \details The Ball's direction must be inverted whenever it collides with a Paddle object or
  * when it touches one of the boundary walls in the game of Pong.
  * \param choice Determines which coordinate value to invert (y-coordinate = 0, x-coordinate = 1).
  * \see Paddle class, Pong class.
  */
void Ball::invert(int choice) {
  if(choice == 0) {
    myYY = -myYY;
  } else if(choice == 1) {
    myXX = -myXX;
    myYY += randfloat(1000) * 2 - 1;
  }
}

 /*!
  * \brief Move the Ball object.
  * \details Actually moves the Ball object around.
  */
void Ball::move() {
  myX += myXX;
  myY += myYY;
  myCircle->setCenter(myX, myY);
}

 /*!
  * \brief Private helper returning a random float.
  * \details Calculates a random float to return.
  */
float Ball::randfloat(int divisor) {
    return (rand() % divisor) / (float) divisor;
}

 /*!
  * \brief Reset the Ball object's position.
  * \details After a point is earned in the game of Pong, the Ball object's position must be reset back
  * to the middle of the Canvas and its direction must be randomized again.
  * \param can Reference to the Canvas object that has the Ball object.
  */
void Ball::reset(Canvas& can) {
  myX = can.getWindowWidth() / 2-8;
  myY = can.getWindowHeight() / 2-8;
  do {
    myDir = randfloat(1000) * 2 * 3.14159f;
    myXX = mySpeed * cos(myDir);
    myYY = mySpeed * sin(myDir);
  } while (myXX > -4 && myXX < 4);
}
