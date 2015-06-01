/*
 * testSpiral.cpp
 *
 *  Created on: May 29, 2015
 *      Author: cpd5
 */

#include "tsgl.h"

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;
const int NUM_COLORS = 256, MAX_COLOR = 255;


void newtonPendulumFunction(Canvas& can) {
  const int WINDOW_W = can.getWindowWidth(), WINDOW_H = can.getWindowHeight(),
  RADIUS = WINDOW_W / 2 - 175;  //Radius of circles
  int centerX = WINDOW_W / 2;   //Center of window
  int centerY = WINDOW_H / 2;
  bool leftMoving, rightMoving;  //booleans for seeing if the left or right ball is moving
  while(can.getIsOpen()) {
  can.sleep();
  //Stationary lines
  can.drawLine(centerX, 0, centerX, centerY - 22);  //Middle line
  can.drawLine(centerX - 50, 0, centerX - 50, centerY - 22);
  can.drawLine(centerX + 50, 0, centerX + 50, centerY - 22);
  //Stationary balls
  can.drawCircle(centerX, centerY, RADIUS, 32, BLACK, false); //Middle ball
  can.drawCircle(centerX - 50, centerY, RADIUS, 32, BLACK, false);
  can.drawCircle(centerX + 50, centerY, RADIUS, 32, BLACK, false);

  //Right goes first, left stays stationary
  leftMoving = false;
  rightMoving = true;
  int maxStop = 200;
  //Moving balls (right and left)
  //getReps(), two booleans (one for moving, the other not moving), increment/decrement by 50s
  //right needs to be incremented first, then decremented once it gets far enough off of the screen
  //furthest point for right line: centerX + 200
  //furthest point to draw a circle: same as above
  //stop value for line: centerX + 100 (subtract 50 twice)
  //stop value for circle: same as above
  while(rightMoving) {
  can.clear();
  can.drawLine(centerX + 100, 0, centerX + maxStop, centerY - 22);
  can.drawCircle(centerX + maxStop, centerY, RADIUS, 32, BLACK, false);
  maxStop -= 49 + can.getReps();
  if(maxStop <= 100) rightMoving = false; leftMoving = true;
  }
  //left needs to be decremented first, then incremented once it gets far enough off of the screen
  //furthest point for left: centerX - 200
  //furthest point for left: same as above
  //stop value for line: centerX - 100
  //stop value for circle: centerX - 100
  can.drawLine(centerX - 100, 0, centerX - 200, centerY - 22);
  can.drawCircle(centerX - 200, centerY, RADIUS, 32, BLACK, false);

  can.clear();
}
}

int main() {
	Canvas c1(0, 0, 400, 400, BUFFER, "", FRAME);
	c1.setBackgroundColor(WHITE);
	c1.start();
	newtonPendulumFunction(c1);
	c1.close();
}
