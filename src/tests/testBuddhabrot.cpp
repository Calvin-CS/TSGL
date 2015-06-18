/*
 * testBuddhabrot.cpp
 *
 *  Created on: June 18, 2015
 *      Author: Patrick Crain
 */

#include "Mandelbrot/Buddhabrot.h"

using namespace tsgl;

/*!
 * \brief Draws a gradient Mandelbrot set on a CartesianCanvas
 * \details Same as mandelbrotFunction(), but with smoother shading ( see
 * http://linas.org/art-gallery/escape/smooth.html ).
 * \param can, Reference to the CartesianCanvas being drawn to
 * \param numberOfThreads, Reference to the number of threads to use
 */
void buddhabrotFunction(CartesianCanvas& can, unsigned &threads, unsigned depth) {
  Buddhabrot m1(threads, depth);
	m1.draw(can, threads);
}

//Takes in command line argument for the number of threads
int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : 1200;
  int h = (argc > 2) ? atoi(argv[2]) : 900;
  if (w <= 0 || h <= 0) {     //Checked the passed width and height if they are valid
    w = 1200;
    h = 900;                  //If not, set the width and height to a default value
  }
	Cart c16(-1, -1, w, h, -2, -1.125, 1, 1.125, "", FRAME / 2);
	unsigned t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();    //Get the number of threads to use
	if (t == 0)
	  t = omp_get_num_procs();
	unsigned d = (argc > 4) ? atoi(argv[4]) : 1000;
	c16.setBackgroundColor(BLACK);
	c16.start();
	buddhabrotFunction(c16, t, d);
	c16.wait();
}
