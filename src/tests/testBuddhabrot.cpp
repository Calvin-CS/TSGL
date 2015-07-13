/*
 * testBuddhabrot.cpp
 *
 *  Created on: June 18, 2015
 *      Author: Patrick Crain
 */

#include "Mandelbrot/Buddhabrot.h"

using namespace tsgl;

/*!
 * \brief Draws a Buddhabrot set on a CartesianCanvas.
 * \details Same as mandelbrotFunction(), but with a different shading algorithm and no
 * ProgressBar.
 * ( see https://en.wikipedia.org/wiki/Buddhabrot ).
 * \param can Reference to the CartesianCanvas being drawn to.
 * \param numberOfThreads Reference to the number of threads to use.
 * \param depth The number of iterations to go to in order to draw the Buddhabrot set.
 */
void buddhabrotFunction(CartesianCanvas& can, unsigned &threads, unsigned depth) {
  Buddhabrot m1(threads, depth);  //Create the Buddhabrot object
  m1.draw(can);  //Draw it
}

//Takes in command line argument for the width and height of the screen as well as
//for the number of threads to use and the number of iterations to draw the Buddhabrot set
int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
  int h = (argc > 2) ? atoi(argv[2]) : 0.75*w;
  if (w <= 0 || h <= 0) {     //Checked the passed width and height if they are valid
    w = 1200;
    h = 900;                  //If not, set the width and height to a default value
  }
  Cart c16(-1, -1, w, h, -2, -1.125, 1, 1.125, "Buddhabrot", FRAME / 2);
  unsigned t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();    //Get the number of threads to use
  if (t == 0)
    t = omp_get_num_procs();
  unsigned d = (argc > 4) ? atoi(argv[4]) : 1000;
  c16.setBackgroundColor(BLACK);
  c16.start();
  buddhabrotFunction(c16, t, d);
  c16.wait();
}
