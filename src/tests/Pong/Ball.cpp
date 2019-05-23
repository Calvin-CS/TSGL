/*
 * Ball.cpp
 */

#include "Ball.h"

Ball::Ball(Canvas& can, int & speed) {
  mySpeed = speed;
  myX = can.getWindowWidth() / 2-8;
  myY = can.getWindowHeight() / 2-8;
  do {
    myDir = randfloat(1000) * 2 * PI;
    myXX = mySpeed * cos(myDir);
    myYY = mySpeed * sin(myDir);
  } while(myXX > -4 && myXX < 4);
}

void Ball::draw(Canvas& can) {
  can.drawCircle(myX, myY, 8, 8, WHITE, true);
}

float Ball::getX() const {
  return myX;
}

float Ball::getY() const {
  return myY;
}
void Ball::invert(int choice) {
  if(choice == 0) {
    myYY = -myYY;
  } else if(choice == 1) {
    myXX = -myXX;
    myYY += randfloat(1000) * 2 - 1;
  }
}

void Ball::move() {
  myX += myXX;
  myY += myYY;
}

float Ball::randfloat(int divisor) {
    return (rand() % divisor) / (float) divisor;
}

void Ball::reset(Canvas& can) {
  myX = can.getWindowWidth() / 2-8;
  myY = can.getWindowHeight() / 2-8;
  do {
    myDir = randfloat(1000) * 2 * 3.14159f;
    myXX = mySpeed * cos(myDir);
    myYY = mySpeed * sin(myDir);
  } while (myXX > -4 && myXX < 4);
}
