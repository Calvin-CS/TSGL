/*
 * testSeaUrchin.cpp
 *
 * Usage: ./testSeaUrchin <numThreads>
 */

#include "tsgl.h"
#include <omp.h>

#include "SeaUrchin/SeaUrchin.h"

using namespace tsgl;

/*!
 * \brief Displays different colored sea urchins and takes a command-line argument.
 * \details Displays different colored sea urchins where each sea urchin is drawn by a thread
 * (the command-line argument is the number of threads to use).
 * It is drawn as follows:
 * - The sea urchins are drawn in a similar way as the line fan in testLineFan.
 * - A class contains all of the necessary data and methods to draw the sea urchins.
 * - A parallel block is created and the process is forked.
 * - The current thread's id number is stored.
 * - When you create the SeaUrchin object:
 *  - Set old x and y-coordinate values and increment them based off of the current thread's id number.
 *  - Set new x and y-coordinate values to 0.
 *  - Assign a color to the current thread.
 *  .
 * - While the Canvas is open:
 *   - Sleep the Canvas' internal timer until the next draw cycle.
 *   - Clear the Canvas.
 *   - When you draw the SeaUrchin onto the Canvas:
 *     - Set a delta value to make the SeaUrchins spin.
 *     - For i to the number of spokes of a SeaUrchin:
 *       - Calculate the new x and y-coordinate values based off of the delta value and the old x and y-coordinate values.
 *       - Draw a spoke of the SeaUrchin on the Canvas.
 *       .
 *     .
 *   .
 * - If the Canvas has been closed, output a message to the console saying "YOU KILLED MY SEA URCHINS! :'(" .
 * .
 * \param can Reference to the Canvas being drawn on.
 * \param threads Reference to the number of threads to use in the process.
 * \see testLineFan, SeaUrchin class.
 */
<<<<<<< HEAD
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
=======
void seaUrchinFunction(Canvas& can, int threads) {
  #pragma omp parallel num_threads(threads)
  {
    SeaUrchin s(can, omp_get_thread_num());   //A thread gets a Sea Urchin
    while(can.isOpen()) {   //Draw loop
      can.sleep();
      can.clear();
      s.draw(can);  //And draws it
>>>>>>> 537c46ba6c9b4aff4c592277352ca791cf994e5a
    }
  }
  std::cout << "YOU KILLED MY SEA URCHINS! :'(" << std::endl;
}

int main(int argc, char * argv[]) {
  int nthreads = (argc > 1) ? atoi(argv[1]) : 16;  //Number of threads
  clamp(nthreads,1,16);                            //Max number of threads is 16
  Canvas c(-1, -1, 885, 230, "Sea Urchins!", FRAME * 2);
  c.setBackgroundColor(BLACK);
  c.run(seaUrchinFunction, nthreads);
}

