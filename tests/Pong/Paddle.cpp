/*
 * Paddle.cpp
 */

#include "Paddle.h"

Paddle::Paddle(Canvas& can, int & speed, int side) {
  mySpeed = speed;
  myDir = myPoints = 0;
  myY = can.getWindowHeight() / 2;
  myRect = new Rectangle(0,0, 24, 64);
  if(side == -1) {  //Left side
    myRect->setColor(BLUE);
    myRect->setCenter(20, myY);
  } else if(side == 1) { //Right side
    myRect->setColor(RED);
    myRect->setCenter(can.getWindowWidth() - 20, myY);
  }
  can.add(myRect);
}

void Paddle::bindings(Canvas& can, int side) {
  if(side == 1) { //Right
    can.bindToButton(TSGL_UP, TSGL_PRESS, [this]() {this->myDir = -1;});
    can.bindToButton(TSGL_DOWN, TSGL_PRESS, [this]() {this->myDir = 1;});
    can.bindToButton(TSGL_UP, TSGL_RELEASE, [this]() {if (this->myDir == -1) this->myDir = 0;});
    can.bindToButton(TSGL_DOWN, TSGL_RELEASE, [this]() {if (this->myDir == 1) this->myDir = 0;});
  } else if(side == -1) { //Left
    can.bindToButton(TSGL_W, TSGL_PRESS, [this] () {this->myDir = -1;});
    can.bindToButton(TSGL_S, TSGL_PRESS, [this] () {this->myDir = 1;});
    can.bindToButton(TSGL_W, TSGL_RELEASE, [this] () {if (this->myDir == -1) this->myDir = 0;});
    can.bindToButton(TSGL_S, TSGL_RELEASE, [this] () {if (this->myDir == 1) this->myDir = 0;});
  }
}

void Paddle::increment() {
  ++myPoints;
}

void Paddle::move() {
  myY += mySpeed * myDir;
  myRect->setCenter( myRect->getX(), myY );
}

int Paddle::getPoints() const {
  return myPoints;
}

float Paddle::getY() const {
  return myY;
}

void Paddle::setDir(int direction) {
  myDir = direction;
}
