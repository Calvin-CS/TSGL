/*
 * testNewtonPendulum.cpp
 *
 * Usage: ./testNewtonPendulum <width> <height> <numBalls>
 */

#include "tsgl.h"
#include "Util.h"

using namespace tsgl;

/*!
 * \brief Simulates Newton's Cradle (or Newton's Pendulum as some call it) ( see http://s.hswstatic.com/gif/newtons-cradle-1.jpg ).
 * \details Simulates Newton's Pendulum in the following way:
 * - User variables store the sizes of each ball, the number of balls (command-line argument), and the radius of each ball.
 *   They also store the acceleration, top speed, and the amount to inverse the direction of the swing of the balls.
 * - Automatic variables store the window width and height, the center of the window, the line length, and the offset of each ball.
 * - Initial positions of the edge balls are determined.
 * - The leftmost ball is stationary; the rightmost is moving at top speed.
 * - While the Canvas has not been closed:
 *  - The Canvas' internal timer is put to sleep until the next drawing loop.
 *  - Conditionals determine the left and right ball motion.
 *  - The animation loop is paused.
 *  - The Canvas is cleared.
 *  - The stationary balls and lines are drawn first, followed by the moving balls and lines.
 *  - The animation is resumed.
 *  .
 * .
 * \param can Reference to the Canvas to draw on.
 * \param numberOfBalls The number of balls to use in the function.
 */
void newtonPendulumFunction(Canvas& can, int numberOfBalls) {
  //User variables
  const int   SIDES = 128,      //Sizes for each circle
              BALLS = numberOfBalls, //Keep this odd
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
  while(can.isOpen()) {
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
      can.drawCircle(CX + RADIUS*2*i, CY, RADIUS, GRAY, true);
    }
    //Draw moving lines and balls!
    //Left
    can.drawLine(CX - OFFSET, 0, CX - OFFSET + LINELEN*sin(leftPos/AMP), LINELEN*cos(leftPos/AMP));
    can.drawCircle(CX - OFFSET + LINELEN*sin(leftPos/AMP),  LINELEN*cos(leftPos/AMP), RADIUS, GRAY, true);
    //Right
    can.drawLine(CX + OFFSET, 0, CX + OFFSET + LINELEN*sin(rightPos/AMP), LINELEN*cos(rightPos/AMP));
    can.drawCircle(CX + OFFSET + LINELEN*sin(rightPos/AMP), LINELEN*cos(rightPos/AMP), RADIUS, GRAY, true);
    can.resumeDrawing();
  }
}

//Takes command line arguments for the width and height of the screen
//as well as for the number of balls
int main(int argc, char * argv[]) {
  //Width and height
  int w = (argc > 1) ? atoi(argv[1]) : 1.2 * Canvas::getDisplayHeight();
  int h = (argc > 2) ? atoi(argv[2]) : 0.75 * w;
  int numberOfBalls = atoi(argv[3]);  //Number of balls
  if(w <= 0 || h <= 0) {
    w = 1200;
    h = 900;
  }
  //Determine if the number of balls is valid
  if(numberOfBalls <= 0) {  //If negative, set it to 7
    numberOfBalls = 7;
  } else if(numberOfBalls % 2 == 0) { //If even, add 1
    numberOfBalls++;
  }
  Canvas c(-1, -1, w, h, "Newton's Pendulum");
  c.setBackgroundColor(WHITE);
  c.run(newtonPendulumFunction,numberOfBalls);
}
