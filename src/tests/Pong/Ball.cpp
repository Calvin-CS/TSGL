/*
 * Ball.cpp
 *
 *  Created on: Jul 14, 2015
 *      Author: cpd5
 */

#include "Ball.h"

Ball::Ball(Canvas& can) {
  mySpeed = 8;
  myX = can.getWindowWidth() / 2 - 8;
  myY = can.getWindowHeight() / 2 - 8;
  do {
    myDir = randfloat(1000) * 2 * PI;
    myXX = mySpeed * cos(myDir);
    myYY = mySpeed * sin(myDir);
  } while(myXX > -4 && myXX < 4);
}

Ball::~Ball() {
  // TODO Auto-generated destructor stub
}

void Ball::draw(Canvas& can) {
  can.drawRectangle(myX - 8, myY - 8, myX + 8, myY + 8, WHITE);
}

float Ball::getX() const {
  return myX;
}

float Ball::getY() const {
  return myY;
}

void Ball::move() {
  myX += myXX;
  myY += myYY;
}

void Ball::reset() {
  do {
    myDir = randfloat(1000) * 2 * 3.14159f;
    myXX = mySpeed * cos(myDir);
    myYY = mySpeed * sin(myDir);
  } while (myXX > -4 && myXX < 4);
}

float Ball::randfloat(int divisor) {
    return (rand() % divisor) / (float) divisor;
}
