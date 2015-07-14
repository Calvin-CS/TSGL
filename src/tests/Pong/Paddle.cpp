/*
 * Paddle.cpp
 *
 *  Created on: Jul 14, 2015
 *      Author: cpd5
 */

#include "Paddle.h"

Paddle::Paddle(Canvas& can) {
  myDir = myPoints = 0;
  myY = can.getWindowWidth() / 2 - 32;
}

void Paddle::bindings(Canvas& can, int side) {
  if(side == -1) { //Left
    can.bindToButton(TSGL_UP, TSGL_PRESS, [this]() {this->myDir = -1;});
    can.bindToButton(TSGL_DOWN, TSGL_PRESS, [this]() {this->myDir = 1;});
    can.bindToButton(TSGL_UP, TSGL_RELEASE, [this]() {if (this->myDir == -1) this->myDir = 0;});
    can.bindToButton(TSGL_DOWN, TSGL_RELEASE, [this]() {if (this->myDir == 1) this->myDir = 0;});
  } else if(side == 1) { //Right
    can.bindToButton(TSGL_W, TSGL_PRESS, [this] () {this->myDir = -1;});
    can.bindToButton(TSGL_S, TSGL_PRESS, [this] () {this->myDir = 1;});
    can.bindToButton(TSGL_W, TSGL_RELEASE, [this] () {if (this->myDir == -1) this->myDir = 0;});
    can.bindToButton(TSGL_S, TSGL_RELEASE, [this] () {if (this->myDir == 1) this->myDir = 0;});
  }
}

void Paddle::checkBoundColl(Ball & b, Canvas& can, int side) {
  if(side == -1) { //Left

  } else if(side == 1) {  //Right

  }
}

void Paddle::checkPadColl(Ball & b, Canvas& can, int side) {

}
void Paddle::draw(Canvas& can, int side) {
  if(side == -1) {  //Left side
    can.drawRectangle(8, myY, 32, myY + 64, ColorFloat(0.0f, 0.0f, 1.0f, 1.0f));
  } else if(side == 1) { //Right side
    can.drawRectangle(can.getWindowWidth() - 24 - 8, myY, can.getWindowWidth() - 8, myY + 64, ColorFloat(1.0f, 0.0f, 0.0f, 1.0f));
  }
}

void Paddle::increment() {
  ++myPoints;
}

void Paddle::move() {
  myY += 4 * myDir;
}

int Paddle::getPoints() const {
  return myPoints;
}

void Paddle::setDir(int direction) {
  myDir = direction;
}

Paddle::~Paddle() {
  // TODO Auto-generated destructor stub
}

