/*
 * testNewtonPendulum.cpp
 *
 *  Created on: May 29, 2015
 *      Authors: Chris Dilley, Patrick Crain
 */

#include "tsgl.h"

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

void newtonPendulumFunction(Canvas& can) {
  const int WINDOW_W = can.getWindowWidth(), WINDOW_H = can.getWindowHeight(),
      RADIUS = WINDOW_W / 2 - 175;  //Radius of circles
  int centerX = WINDOW_W / 2;   //Center of window
  int centerY = WINDOW_H / 2;
  int leftMoving, rightMoving;  //booleans for seeing if the left or right ball is moving
  //Right goes first, left stays stationary
  int rightStop = 200;
  int leftStop = -100;
  leftMoving = 0;
  rightMoving = -1;
  while(can.getIsOpen()) {
    can.sleep();
    can.clear();
    //Stationary lines
    can.drawLine(centerX, 0, centerX, centerY - 21);  //Middle line
    can.drawLine(centerX - 50, 0, centerX - 50, centerY - 21);
    can.drawLine(centerX + 50, 0, centerX + 50, centerY - 21);
    //Stationary balls
    can.drawCircle(centerX, centerY, RADIUS, 32, BLACK, false); //Middle ball
    can.drawCircle(centerX - 50, centerY, RADIUS, 32, BLACK, false);
    can.drawCircle(centerX + 50, centerY, RADIUS, 32, BLACK, false);

    //Moving balls (right and left)
    //getReps(), two booleans (one for moving, the other not moving), increment/decrement by 50s
    //right needs to be incremented first, then decremented once it gets far enough off of the screen
    //Farthest point for right line: centerX + 200
    //Farthest point to draw a circle: same as above
    //stop value for line: centerX + 100 (subtract 50 twice)
    //stop value for circle: same as above
    can.drawLine(centerX + 100, 0, centerX + rightStop, centerY - 21);
    can.drawCircle(centerX + rightStop, centerY + 1, RADIUS, 32, BLACK, false);
    if(rightMoving != 0) {
      rightStop += 10 * rightMoving + 3;
      if(rightStop <= 100) {
        leftMoving = -1;
        rightMoving = 0;
        rightStop = 100;
      } else if(rightStop >= 200) {
        rightMoving = -1;
        rightStop = 200;
      }
    }

    can.drawLine(centerX - 100, 0, centerX + leftStop, centerY - 21);
    can.drawCircle(centerX + leftStop, centerY + 1, RADIUS, 32, BLACK, false);
    if(leftMoving != 0) {
      leftStop += 10 * leftMoving + 3;
      if(leftStop <= -200) {
        leftMoving = 1;
        rightMoving = 0;
        leftStop = -200;
      } else if(leftStop >= -100) {
        rightMoving = 1;
        leftMoving = 0;
        leftStop = -100;
      }
    }
    //left needs to be decremented first, then incremented once it gets far enough off of the screen
    //Farthest point for left: centerX - 200
    //Farthest point for left: same as above
    //stop value for line: centerX - 100
    //stop value for circle: centerX - 100
  }
}

int main() {
  Canvas c1(0, 0, 400, 400, "", FRAME);
  c1.setBackgroundColor(WHITE);
  c1.start();
  newtonPendulumFunction(c1);
  c1.close();
}
