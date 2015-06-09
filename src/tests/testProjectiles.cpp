/*
 * testProjectiles.cpp
 *
 *  Created on: Jun 2, 2015
 *      Author: Chris Dilley
 */

#include "tsgl.h"

using namespace tsgl;

/*!
 * \brief Target practice mini-game!
 * \details Creates the target practice mini-game in this way:
 * - Get coordinates for the target based off of the window size.
 * - Set a coordinate changer for the target's x coordinate (so that it moves from left to right on the screen).
 * - Get the center of the Canvas.
 * - Get the number of targets (10).
 * - Get the score counter.
 * - Get the colors for the target (outer, middle, and bulls-eye center).
 * - Bind the mouse so that when one clicks and hits the target the score counter goes up.
 * - While the Canvas is still open:
 *    - Sleep the internal timer of the Canvas until the next drawing loop is ready to be drawn.
 *    - Increment the targetX by the coordinate changer.
 *    - Move the target up vertically by coordinate changer for y.
 *    - Draw the actual target.
 *    - If the target hits the middle of the screen:
 *     - Invert the y change so that the target moves downward.
 *    - If the target goes off screen:
 *     - Decrement the number of targets left.
 *     - Reset the targetX, targetY, and the coordinate changer for y.
 *    - If we hit a target:
 *     - Increment the score.
 *     - Change the hit to false;
 *     - Move the target off screen.
 *    - If the number of targets is less than 5:
 *     - Speed up the movement of the remaining targets.
 *    - If the number of targets left is 0:
 *     - Print out the score, clear the Canvas one last time, and get out of the drawing loop.
 * - Repeat loop indefinitely until the Canvas has been closed.
 * \param can, Reference to the Canvas being drawn on.
 */
void projectileFunction(Canvas& can) {
  const int WINDOW_W = can.getWindowWidth(), WINDOW_H = can.getWindowHeight();
  int targetX = 0, targetY = WINDOW_W / 2, coordinateChangerY = 1, coordinateChangerX = 3;  //Used to control target motion
  int centerX = WINDOW_W / 2, centerY = WINDOW_H / 2;   //Center of screen
  int numberOfTargets = 10;   //Number of targets
  bool hit = false;  //Determine if the target has been hit
  int score = 0;   //Score
  ColorInt blueTarget(0, 0, 255);
  ColorInt redTarget(255, 0, 0);   //Color for targets
  ColorInt yellowTarget(255, 255, 0);
  //binding to left mouse button
  can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&hit, &can, &targetX, &targetY]() {
    if((can.getMouseX() <= targetX && can.getMouseY() <= targetY) || can.getMouseY() == targetY) {
      hit = true;  //We hit the target
    }
  });

  //Draw loop
  while(can.getIsOpen()) {
    can.sleep();
    targetX += coordinateChangerX;  //Horizontal movement
    targetY -= coordinateChangerY; //Vertical movement
    can.drawCircle(targetX, targetY, 50, 32, blueTarget, true);   //Outer circle
    can.drawCircle(targetX, targetY, 30, 32, redTarget, true);  //Middle
    can.drawCircle(targetX, targetY, 10, 32, yellowTarget, true); //Inner
    if(targetX >= centerX) { //If it hits the middle of the screen, invert the vertical direction
      coordinateChangerY = -1;
    }
    if(targetX > WINDOW_W + 60) {  //If the target is off the screen
      numberOfTargets--;  //Decrement the number of targets left
      targetX = 0;  //Reset the target and inverter
      targetY = 200;
      coordinateChangerY = 1;
    }
    if(hit) {   //If we hit a target....
      score++;
      targetX = WINDOW_W + 60;
      hit = false;
    }
    if(numberOfTargets <= 5) {   //Mix it up if there are only five targets left (speed up the targets)
      coordinateChangerX = 6;
    }

    if(numberOfTargets == 0) {   //End game
      can.clear();
      std::cout << "Your score: " << score << std::endl;
      return;
    }
      can.clear();
   }
    can.clear();
}

//Takes command-line arguments for the width and height
int main(int argc, char* argv[]) {
  int width = atoi(argv[1]), height = atoi(argv[2]);
  Canvas c1(0, 0, width, height, "", FRAME);    //Can change the size of the window
  c1.setBackgroundColor(BLACK);
  c1.start();
  projectileFunction(c1);
  c1.wait();
}
