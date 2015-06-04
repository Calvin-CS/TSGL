/*
 * testNewtonPendulum.cpp
 *
 *  Created on: May 29, 2015
 *      Authors: Chris Dilley, Patrick Crain
 */

#include "tsgl.h"

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

/*!
 * \brief Simulates Newton's Cradle (or Newton's Pendulum as some call it) ( see http://s.hswstatic.com/gif/newtons-cradle-1.jpg ).
 * \details Simulates Newton's Pendulum in the following way:
 * - A Canvas is made and the center of it is determined.
 * - Stopping points for the left and right moving balls are determined.
 * - Ints determining whether or not a ball is moving are also made (-1 = moving left, 0 = stationary, 1 = moving right).
 * - While the Canvas has not been close:
 *  - The Canvas' internal timer is put to sleep until the next drawing loop is ready to be made on the Canvas.
 *  - The Canvas is cleared (not necessary for the first frame but necessary for the subsequent frames afterwards).
 *  - Three circles are made with respect to the center of the Canvas.
 *  - The middle circle is the first to be drawn, followed by the left and then the right.
 *  - Then, lines are drawn from those circles so that drawn image looks like the middle three stationary balls of Newton's Pendulum.
 *  - Now, two more circles and lines are drawn. These will be the ones moving from right to left.
 *  - Conditonals determine whether to increment the motion of the left or right ball.
 * - The drawing loop continues on indefinitely until the Canvas is closed.
 * \param can Reference to the Canvas to draw on.
 */
void newtonPendulumFunction(Canvas& can) {
  const int WINDOW_W = can.getWindowWidth(), WINDOW_H = can.getWindowHeight(),
      RADIUS = WINDOW_W / 2 - 175;  //Radius of circles
  int centerX = WINDOW_W / 2;   //Center of window
  int centerY = WINDOW_H / 2;
  int leftMoving, rightMoving;  //ints for seeing if the left or right ball is moving
  int rightStop = 200;    //Stopping points for the left and right ball
  int leftStop = -100;
  leftMoving = 0;           //Right goes first, left stays stationary
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

    //Drawing conditional for right ball motion
    can.drawLine(centerX + 100, 0, centerX + rightStop, centerY - 21);
    can.drawCircle(centerX + rightStop, centerY + 1, RADIUS, 32, BLACK, false);
    if(rightMoving != 0) {   //If the ball isn't stationary
      rightStop += 10 * rightMoving + 3;  //Move it
      if(rightStop <= 100) {  //If its hit the stopping point
        leftMoving = -1;   //Make it stationary and make the left ball move
        rightMoving = 0;
        rightStop = 100;    //Reset the stopping point
      } else if(rightStop >= 200) {  //This is when the right ball is moving to the right (so that it can move to the left and "hit" the other balls)
        rightMoving = -1;
        rightStop = 200;
      }
    }

    //Drawing conditional for left ball motion
    //Similar to the right ball motion, but with the left stop value and left ball
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
  }
}

int main() {
  Canvas c1(0, 0, 400, 400, "", FRAME); //THIS MUST STAY EXACTLY THE SAME
  c1.setBackgroundColor(WHITE);
  c1.start();
  newtonPendulumFunction(c1);
  c1.close();
}
