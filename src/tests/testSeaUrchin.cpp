/*
 * testSeaUrchin.cpp
 *
 *  Created on: May 29, 2015
 *      Author: cpd5
 */

#include "tsgl.h"
#include "SeaUrchin.h"
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
 *    - After the sea ]urchins have been drawn, draw the spotlights in the top left
 *      and bottom right corners.
 *    - Clear the Canvas.
 *    .
 *- If the Canvas has been closed, output a message to the console saying "YOU KILLED MY SEA URCHINS! :'(" .
 *  .
 * .
 * \param can Reference to the Canvas being drawn on.
 */
void seaUrchinFunction(Canvas& can, int & threads, int colorScheme) {
  srand(time(NULL));
#pragma omp parallel num_threads(threads)
  {
  int id = omp_get_thread_num();
  SeaUrchin s1(can, id, colorScheme);   //A thread gets a Sea Urchin
  while(can.getIsOpen()) {   //Draw loop
    can.sleep();
    can.clear();
//    can.pauseDrawing();
    s1.draw(can);  //And draws it
    #pragma omp barrier
//    can.resumeDrawing();
  }
  }
  std::cout << "YOU KILLED MY SEA URCHINS! :'(" << std::endl;
}

//KEEP THE WINDOW WIDTH AND HEIGHT THE SAME PLEASE!
int main(int argc, char * argv[]) {
  int nthreads = (argv > 1) ? atoi(argv[1]) : 32;  //Number of threads
  int colorChoice = (argv > 2) ? atoi(argv[2]) : 0; //0 = all same color, 1 = all different color
  if(nthreads > 32 || nthreads < 0) {
    nthreads = 32;  //For now...
  }
  if(colorChoice != 0 && colorChoice != 1) {
    colorChoice = 0; //Default
  }
  Canvas c1(-1, -1, 880, 445, "Sea Urchins!", FRAME);
  c1.setBackgroundColor(BLACK);
  c1.start();
  seaUrchinFunction(c1, nthreads, colorChoice);
  c1.wait();
}

