/*
 * testSeaUrchin.cpp
 *
 *  Created on: May 29, 2015
 *      Author: cpd5
 */

#include "tsgl.h"
#include <omp.h>
using namespace tsgl;

/*!
 * \brief Displays different colored sea urchins.
 * \details Displays different colored sea urchins with spotlights in the
 *  top left and bottom right corners. Two of the sea urchins have random colors.
 * It is drawn as follows:
 * - The sea urchins are drawn in a similar way as the line fan in testLineFan.
 * - Get an old x and old y value and a new x and new y value.
 * - Set the new x and y to 0, don't set the old x and y.
 * - Get the colors of the sea urchins.
 * - While the Canvas has not been closed:
 *    - Sleep the internal timer until the next draw cycle.
 *    - Set the old x and old y to half of the window width.
 *    - Draw the first sea urchin.
 *    - Reset the old x and old y to some new coordinates.
 *    - Draw the second sea urchin.
 *    - Continue the pattern until all sea urchins are drawn.
 *    - After the sea urchins have been drawn, draw the spotlights in the top left
 *      and bottom right corners.
 *    - Clear the Canvas.
 *    .
 *- If the Canvas has been closed, output a message to the console saying "YOU KILLED MY SEA URCHINS! :'(" .
 *  .
 * .
 * \param can Reference to the Canvas being drawn on.
 */
void seaUrchinFunction(Canvas& can) {
  int oldX, oldY;    //Coordinates to draw them
  int newX = 0, newY = 0;
  ColorFloat red(1.0f, 0, 0);   //Colors of first three sea urchins
  ColorFloat green(0, 1.0f, 0);
  ColorFloat blue(0, 0, 1.0f);
  ColorFloat yellow(1.0f, 1.0f, 0);   //Spotlight color
  srand(time(NULL));
  int randomColorR = rand() % 255;   //Getting the random color for the last few
  int randomColorG = rand() % 255;   //sea urchins
  int randomColorB = rand() % 255;
  ColorInt random(randomColorR, randomColorG, randomColorB);
  ColorInt random2(randomColorB, randomColorR, randomColorG);
  float delta = 2.0f / 32 * 3.1415926585f;   //Makes them spin
  while(can.isOpen()) {   //Draw loop
    can.sleep();
    oldX = oldY = can.getWindowWidth() / 2;
    //First sea urchin
    for(int i = 0; i < 32; ++i) {
      newX = oldX + 50 * cos(i * delta + can.getReps());
      newY = oldY + 50 * sin(i * delta + can.getReps());
      can.drawLine(oldX, oldY, newX, newY, red);
    }
    //Second sea urchin
    oldX = 150;  //Consider these as the center of the sea urchin
    oldY = 300;
    for(int j = 0; j < 32; ++j) {
      newX = oldX + 50 * cos(j * delta - can.getReps());
      newY = oldY + 100 * sin(j * delta - can.getReps());
      can.drawLine(oldX, oldY, newX, newY, blue);
    }
    //Third sea urchin
    oldX = 300;
    oldY = 50;
    for(int k = 0; k < 32; ++k) {
      newX = oldX + 50 * cos(k * delta + can.getReps());
      newY = oldY + 50 * sin(k * delta + can.getReps());
      can.drawLine(oldX, oldY, newX, newY, green);
    }
    //Fourth sea urchin
    oldX = 50;
    oldY = 400;
    for(int l = 0; l < 32; ++l) {
      newX = oldX + 100 * cos(l * delta + can.getReps());
      newY = oldY + 150 * sin(l * delta + can.getReps());
      can.drawLine(oldX, oldY, newX, newY, green);
    }
    //Fifth sea urchin
    oldX = 400;
    oldY = 200;
    for(int m = 0; m < 32; ++m) {
      newX = oldX + 100 * cos(m * delta - can.getReps());
      newY = oldY + 150 * sin(m * delta - can.getReps());
      can.drawLine(oldX, oldY, newX, newY, random);
    }
    //Last sea urchin
    oldX = 240;
    oldY = 390;
    for(int n = 0; n < 32; ++n) {
      newX = oldX + 50 * cos(n * delta - can.getReps());
      newY = oldY + 50 * sin(n * delta - can.getReps());
      can.drawLine(oldX, oldY, newX, newY, random2);
    }
    //"Spotlights" from the corners
    oldX = oldY = 0;    //First spotlight, top left corner
    int oldX2 = can.getWindowWidth(), oldY2 = can.getWindowHeight();   //Second, bottom right corner
    int newX2 = 0, newY2 = 0;
    for(int o = 0; o < 32; ++o) {
      newX = oldX + 50 * cos(o * delta + can.getReps()) + 100;
      newY = oldY + 50 * sin(o * delta + can.getReps()) + 100;
      can.drawLine(oldX, oldY, newX, newY, yellow);
      newX2 = oldX2 + 50 * cos(o * delta + can.getReps()) - 100;
      newY2 = oldY2 + 50 * sin(o * delta + can.getReps()) - 100;
      can.drawLine(oldX2, oldY2, newX2, newY2, yellow);
    }
    can.clear();
  }
  std::cout << "YOU KILLED MY SEA URCHINS! :'(" << std::endl;
}

//KEEP THE WINDOW WIDTH AND HEIGHT THE SAME PLEASE!
int main() {
  Canvas c1(-1, -1, 500, 500, "Sea Urchins in the Spotlight!", FRAME);
  c1.setBackgroundColor(BLACK);
  c1.start();
  seaUrchinFunction(c1);
  c1.wait();
}

