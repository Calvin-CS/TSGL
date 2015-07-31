/*
 * LangtonAnt.cpp
 */

#include "LangtonAnt.h"

LangtonAnt::LangtonAnt(int x, int y, int r, int g, int b, int d, AntFarm* p) {
  myX = x;
  myY = y;
  myRed = r;
  myGreen = g;
  myBlue = b;
  myDir = d;
  myFarm = p;
  myAlpha = 16;
}

void LangtonAnt::move() {
  switch (myDir) {
      case UP:
          myY = (myY > 0) ? myY - 1 : myFarm->height - 1;
          break;
      case RIGHT:
          myX = (myX < myFarm->width - 1) ? myX + 1 : 0;
          break;
      case DOWN:
          myY = (myY < myFarm->height - 1) ? myY + 1 : 0;
          break;
      case LEFT:
          myX = (myX > 0) ? myX - 1 : myFarm->width - 1;
          break;
      default:
          break;
  }
}

void LangtonAnt::changeColor(int r, int g, int b) {
  myRed = r; myGreen = g; myBlue = b;
}

void LangtonAnt::changeColor(ColorFloat c) {
  changeColor(c.R*255,c.G*255,c.B*255);
}

void LangtonAnt::setAlpha(int a) {
  myAlpha = a;
}
