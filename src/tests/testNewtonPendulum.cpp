/*
 * testNewtonPendulum.cpp
 *
 *  Created on: May 29, 2015
 *      Authors: Chris Dilley, Patrick Crain
 */

#include "tsgl.h"

using namespace tsgl;

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

/*!
 * \brief Simulates Newton's Cradle (or Newton's Pendulum as some call it) ( see http://s.hswstatic.com/gif/newtons-cradle-1.jpg ).
 * \details Simulates Newton's Pendulum in the following way:
 * - A Canvas is made and the center of it is determined.
 * - Stopping points for the left and right moving balls are determined.
 * - Ints determining whether or not a ball is moving are also made (-1 = moving left, 0 = stationary, 1 = moving right).
 * - While the Canvas has not been closed:
 *  - The Canvas' internal timer is put to sleep until the next drawing loop.
 *  - Three circles are made with respect to the center of the Canvas.
 *  - The middle circle is the first to be drawn, followed by the left and then the right.
 *  - Then, lines are drawn from those circles so that drawn image looks like the middle three stationary balls of Newton's Pendulum.
 *  - Now, two more circles and lines are drawn. These will be the ones moving from right to left.
 *  - Conditionals determine whether to increment the motion of the left or right ball.
 *  - The Canvas is cleared.
 *  - The drawing loop continues on indefinitely until the Canvas is closed.
 *  .
 * .
 * \param can Reference to the Canvas to draw on.
 */
void newtonPendulumFunction(Canvas& can) {
  //User variables
  const int   SIDES = 128,      //Sizes for each circle
              BALLS = 7,        //Keep this odd
              RADIUS = 20;      //Radius of circles
  const float ACCEL = 0.5f,     //Acceleration of balls
              TOPSPEED = 9.0f, //Starting speed of balls
              AMP = 100;        //Inverse amplitude of balls

  //Automatic variables
  const int   WINDOW_W = can.getWindowWidth(),
              WINDOW_H = can.getWindowHeight(),
              CX = WINDOW_W / 2,   //Center of window
              CY = WINDOW_H / 2;
  const float LINELEN = CY,
              OFFSET = RADIUS*(BALLS-1);

  //Computation
  float rightPos = 0, leftPos = 0;               //Initial positions of the edge balls
  float leftMoving = 0, rightMoving = TOPSPEED;  //Right goes first, left stays stationary
  while(can.getIsOpen()) {
    can.sleep();

    //Drawing conditional for right ball motion
    if(rightMoving != 0 || rightPos != 0) {   //If the ball isn't stationary
      rightMoving -= ACCEL;
      rightPos += rightMoving;  //Move it
      if(rightPos < 0) {  //If its hit the stopping point
        leftMoving = -TOPSPEED;   //Make it stationary and make the left ball move
        rightMoving = 0;
        rightPos = 0;    //Reset the stopping point
      }
    }
    //Drawing conditional for left ball motion
    //Similar to the right ball motion, but with the left stop value and left ball
    if(leftMoving != 0 || leftPos != 0) {
      leftMoving += ACCEL;
      leftPos += leftMoving;
      if(leftPos > 0) {
        rightMoving = TOPSPEED;
        leftMoving = 0;
        leftPos = 0;
      }
    }

    can.pauseDrawing();
    can.clear();
    //Draw stationary lines and balls
    for (float i = -(BALLS/2)+1; i < BALLS/2; ++i) {
      can.drawLine(CX + RADIUS*2*i, 0, CX + RADIUS*2*i, LINELEN);
      can.drawCircle(CX + RADIUS*2*i, CY, RADIUS, SIDES, GREY, true);
    }
    //Draw moving lines and balls!
    //Left
    can.drawLine(CX - OFFSET, 0, CX - OFFSET + LINELEN*sin(leftPos/AMP), LINELEN*cos(leftPos/AMP));
    can.drawCircle(CX - OFFSET + LINELEN*sin(leftPos/AMP),  LINELEN*cos(leftPos/AMP), RADIUS, SIDES, GREY, true);
    //Right
    can.drawLine(CX + OFFSET, 0, CX + OFFSET + LINELEN*sin(rightPos/AMP), LINELEN*cos(rightPos/AMP));
    can.drawCircle(CX + OFFSET + LINELEN*sin(rightPos/AMP), LINELEN*cos(rightPos/AMP), RADIUS, SIDES, GREY, true);
    can.resumeDrawing();
  }
}

int main() {
  Canvas c1(-1, -1, 600, 400, "Newton's Pendulum"); //THIS MUST STAY EXACTLY THE SAME
  c1.setBackgroundColor(WHITE);
  c1.start();
  newtonPendulumFunction(c1);
  c1.wait();
}
