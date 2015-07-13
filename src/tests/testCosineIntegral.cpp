/*
 * testCosineIntegral.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>
#include "Util.h"  //Constants

using namespace tsgl;

typedef CartesianCanvas Cart;

/*!
 * \brief Draws the area under a predefined function (the integral) using CartesianCanvas
 * and takes a command line argument for the number of threads to use.
 * \details
 * - Use the number of threads passed via command-line arguments.
 * - If that number is negative, just use one thread.
 * - Set up the internal timer of the Canvas to expire once every \b FRAME / 2 seconds .
 * - Draw axes through the origin, with spacing PI/4 between x ticks and 0.5 between y ticks.
 * - Store the width of the canvas's pixel in \b pw to avoid thousands of multiple function calls.
 * - Initialize and draw a CosineFunction using the currently rendered area of the CartesianCanvas.
 * - Set the CartesianCanvas' font from an external font file using setFont().
 * - Draw some labels on the CartesianCanvas to make things look pretty.
 * - Set up a parallel block with OMP using \b threads as the number of threads to use.
 * - Set \b nthreads to the actual number of threads spawned.
 * - Calculate each thread's share of the work and store it in: \b offset.
 * - Calculate each thread's starting position and store it in: \b start.
 * - Calculate each thread's stopping position and store it in: \b stop.
 * - For each thread, from \b start to \b stop with step size \b pw:
 *   - If the Canvas was closed, break.
 *   - Sleep the internal timer until it's ready to render.
 *   - Draw a line from x,0 to x,f(x) for the current x.
 *   .
 * .
 * \param can Reference to the CartesianCanvas being drawn to.
 * \param numberOfThreads Reference to the number of threads to use.
 */
void cosineIntegralFunction(Cart& can, int & numberOfThreads) {
  int threads = numberOfThreads;
  if (threads <= 0) {
    threads = 1;
  }

  can.drawAxes(0, 0, PI/4, .5);
  long double pw = can.getPixelWidth();
  CosineFunction function1;
  can.drawFunction(function1);

  can.setFont("../assets/freefont/FreeSerif.ttf");
  can.drawText(L"-1.5π", -1.5 * PI - .1, .25, 20);  // Note the important capital L, used to support Unicode.
  can.drawText(L"1.5\u03C0", 1.5 * PI - .2, .25, 20);
  can.drawText(L"1", .1, 1.05, 20);
  can.drawText(L"-1", .1, -1.1, 20);

#pragma omp parallel num_threads(threads)
  {
    int nthreads = omp_get_num_threads();
    long double offset = 3*PI / nthreads;
    long double start = -1.5*PI + omp_get_thread_num() * offset;
    long double stop = start + offset;
    for (long double i = start; i < stop; i += pw) {
      if (!can.getIsOpen()) break;
      can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
      can.drawLine(i, 0, i, function1.valueAt(i), Colors::highContrastColor(omp_get_thread_num()));
    }
  }
}

//Takes in command line arguments for the width and height of the window
//as well as the number of threads to use
int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
  int h = (argc > 2) ? atoi(argv[2]) : 0.75*w;
  if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
    w = h = 960;              //If not, set the width and height to a default value
  Cart c12(-1, -1, w, h, -5,-1.5,5,1.5, "Cosine Integral", FRAME / 2);
  int t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();   //Number of threads to use
  c12.setBackgroundColor(WHITE);
  c12.start();
  cosineIntegralFunction(c12, t);   //Pass the argument
  c12.wait();
}
