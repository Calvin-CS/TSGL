/*
 * testClock.cpp
 *
 *  Created on: May 29, 2015
 *      Author: cpd5
 */

#include "tsgl.h"

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

typedef CartesianCanvas Cart;

//Testbed for fixing CartesianCanvas
//Will be a Clock test once CartesianCanvas methods draw correctly
//(see "Clean up little TSGL issues" on Trello
void clockFunction(Cart& can) {
  can.drawAxes(0, 0, 10, 10);
  can.drawRectangle(200, 60, 50, 100, BLACK, false);
}

int main() {
  Cart c1;
  c1.setBackgroundColor(WHITE);
  c1.start();
  clockFunction(c1);
  c1.close();
}

