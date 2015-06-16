/*
 * testClock.cpp
 *
 *  Created on: May 29, 2015
 *      Author: cpd5
 */

#include "tsgl.h"

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

using namespace tsgl;


void seaUrchinFunction(Canvas& can) {
  const int CENTER_X = can.getWindowWidth() / 2, CENTER_Y = can.getWindowHeight() / 2;
  while(can.getIsOpen()) {
    srand(time(NULL));
    can.sleep();
    float delta = 2.0f / 32 * 3.1415926585f;
    int x = 250;
    int y = 250;
    int oldX = 250;
    int oldY = 250;
    int newX, newY = 0;
    for(int i = 0; i < 32; ++i) {
      //Plus = clockwise
      //Minus = counterclockwise
      newX = oldX + 50 * cos(i *delta + can.getReps());
      newY = oldY + 200* sin(i *delta + can.getReps());
      can.drawLine(oldX, oldY, newX, newY);
      can.clear();
    }
    newX = newY = 0;
    oldX = oldY = 150;
    for(int j = 0; j < 64; ++j) {
      newX = oldX + 50 * cos(j *delta + can.getReps());
      newY = oldY + 50 * sin(j *delta + can.getReps());
      can.drawLine(oldX, oldY, newX, newY);
      can.clear();
    }

    oldX = 50;
    oldY = 400;
    for(int k = 0; k < 128; ++k) {
      newX = oldX + 50 * cos(k *delta + can.getReps());
      newY = oldY + 50 * sin(k *delta + can.getReps());
      can.drawLine(oldX, oldY, newX, newY);
      can.clear();
    }
  }
}

int main() {
  Canvas c1(0, 0, 500, 500, "", FRAME);
  c1.setBackgroundColor(WHITE);
  c1.start();
  seaUrchinFunction(c1);
  c1.wait();
}

