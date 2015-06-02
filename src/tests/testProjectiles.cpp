/*
 * testProjectiles.cpp
 *
 *  Created on: Jun 2, 2015
 *      Author: cpd5
 */

#include "tsgl.h"

//It'll actually be a target practice game
//Targets will appear on screen
//10 targets
//bindings to mouse click
//score counter
//Increment speed of targets moving....
//Idea for darkness game:
// -Canvas will have an all black background
// -Console will tell user where to click
// -Console will also give current coordinates
// -User has to map to them perfectly
// -If the coordinates match perfectly when the user clicks, the screen turns
//  all white and whatever was in the darkness will appear.
void projectileFunction(Canvas& can) {
  //Data section...
  const int WINDOW_W = can.getWindowWidth(), WINDOW_H = can.getWindowHeight();  //Window width and height
  int circleX = 0, circleY = 200, changer = 1, coordinateChanger = 3;  //Used to control target motion
  int centerX = WINDOW_W / 2, centerY = WINDOW_H / 2;   //Center of screen
  int numberOfTargets = 10;   //Number of targets
  bool hit = false;  //Determine if the target has been hit
  int score = 0;   //Score
  ColorInt red(255, 0, 0);   //Color for targets
  ColorInt blue(0, 0, 255);
  ColorInt yellow(255, 255, 0);
  can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&hit, &can, &circleX, &circleY]() {
    if((can.getMouseX() <= circleX && can.getMouseY() <= circleY) || can.getMouseY() == circleY) {
      hit = true;
    }
  });

  //Draw loop
  while(can.getIsOpen()) {
    can.sleep();
    circleX += coordinateChanger;  //Horizontal movement
    circleY -= changer; //Vertical movement
    can.drawCircle(circleX, circleY, 50, 32, blue, true);   //Outer circle
    can.drawCircle(circleX, circleY, 30, 32, red, true);  //Middle
    can.drawCircle(circleX, circleY, 10, 32, yellow, true); //Inner
    if(circleX >= centerX) { //If it hits the middle of the screen, invert the directio
      changer = -1;
    }
    if(circleX > WINDOW_W + 60) {  //If the target is off the screen
      numberOfTargets--;  //Decrement the number of targets left
      circleX = 0;  //Reset the target and inverter
      circleY = 200;
      changer = 1;
    }
    if(hit) {
      score++;
      circleX = WINDOW_W + 60;
      hit = false;
    }
    if(numberOfTargets <= 5) {   //Mix it up if there are only five targets left
      coordinateChanger = 6;
    }

    if(numberOfTargets == 0) {
      can.clear();
      std::cout << "Your score: " << score;
      return;
    }
      can.clear();
   }
    can.clear();
}

int main() {
  Canvas c1(0, 0, 400, 400, "", FRAME);
  c1.setBackgroundColor(BLACK);
  c1.start();
  projectileFunction(c1);
  c1.close();
}
