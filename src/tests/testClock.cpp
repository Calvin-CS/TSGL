/*
 * testClock.cpp
 *
 *  Created on: May 29, 2015
 *      Author: cpd5
 */

#include "tsgl.h"

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

using namespace tsgl;


void clockFunction(Canvas& can) {
  const int CENTER_X = can.getWindowWidth() / 2, CENTER_Y = can.getWindowHeight() / 2;
  while(can.getIsOpen()) {
   can.sleep();
   float delta = 2.0f / 32 * 3.1415926585f;
   int x = 250;
   int y = 250;
   int oldX = 250;
   int oldY = 250;
   int newX, newY = 0;
   for(int i = 0; i < 32; ++i) {
     newX = oldX + 50 * cos(i*delta);
     newY = oldY + 50 * sin(i*delta);
     can.drawLine(oldX, oldY, newX, newY);
//    can.drawCircle(CENTER_X, CENTER_Y, 200, 32, BLACK, false);
//    can.drawLine(CENTER_X, CENTER_Y, CENTER_X, CENTER_Y - 150);
//    can.drawLine(CENTER_X, CENTER_Y, CENTER_X + 100, CENTER_Y);
   }
  }
}
int main() {
  Canvas c1(0, 0, 500, 500, "", FRAME);
  c1.setBackgroundColor(WHITE);
  c1.start();
  clockFunction(c1);
  c1.wait();
}

