/*
 * testClock.cpp
 *
 *  Created on: May 29, 2015
 *      Author: cpd5
 */

#include "tsgl.h"

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

void clockFunction(Canvas& can) {
  const int WINDOW_W = can.getWindowWidth(),
            WINDOW_H = can.getWindowHeight(),
            RADIUS = WINDOW_W / 2 - 32;
  int centerX = WINDOW_W / 2;
  int centerY = WINDOW_H / 2;
  can.drawCircle(centerX, centerY, RADIUS, 64, BLACK, false);
  can.drawLine(centerX, centerY * 2, centerX, centerY / 2 - centerY);
  can.drawLine(centerX / 2 - centerX, centerY, centerX * 2, centerY);
  can.drawLine(centerX + 20, centerY, centerX + 166, centerY - 32);
}

int main() {
  Canvas c1(0, 0, 400, 400, "");
  c1.setBackgroundColor(WHITE);
  c1.start();
  clockFunction(c1);
  c1.close();
}

