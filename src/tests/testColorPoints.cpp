/*
 * testColorPoints.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <omp.h>
#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws a neat pattern of points to a Canvas using OMP and takes in a command line
 * argument for the number of threads to use.
 * \details
 * - A parallel block is set up with #pragma omp parallel using the number of threads passed.
 * - The actual number of threads created is stored in: \b nthreads .
 * - The number of lines per thread is calculated and stored in: \b myPart .
 * - The starting position of each given thread is calculated and stored in: \b myStart .
 * - The outer for loop is set up in a block pattern, and the inner for loop runs from 0 to the Canvas width.
 * - The color for a given point is calculated.
 * - The point is drawn to the Canvas.
 * - The function breaks from the outer for loop if the Canvas is closed.
 * .
 * \param can Reference to the Canvas being drawn to.
 * \param numberOfThreads Reference to the number of threads to use.
 */
void colorPointsFunction(Canvas& can, int & numberOfThreads) {
#pragma omp parallel num_threads(numberOfThreads)
  {
    int nthreads = omp_get_num_threads();  //Actual number of threads to use
    int myPart = can.getWindowHeight() / nthreads;
    int myStart = myPart * omp_get_thread_num();
    for (int j = myStart; j < myStart + myPart; j++) {
      for (int i = 0; i < can.getWindowWidth(); i++) {
        if (i % 2 == 0)
          can.drawPoint(i, j, ColorInt(j % NUM_COLORS, i % NUM_COLORS, (i * j) % 113));
        else
          can.drawPoint(i, j, ColorInt(i % NUM_COLORS, j % NUM_COLORS, (i * j) % NUM_COLORS));
      }
      if (!can.getIsOpen()) break;
    }
  }
}

//Takes in command line arguments for the window width and height as well
//as for the number of threads to use
int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
  int h = (argc > 2) ? atoi(argv[2]) : w;
  if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
    w = h = 960;              //If not, set the width and height to a default value
  //Convert the char pointer to an int, http://www.cplusplus.com/forum/beginner/58493/
  int t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();
  Canvas c1(-1, -1, w, h, "Blue and Yellow, Blue and Yellow");
  c1.start();
  colorPointsFunction(c1, t);   //Now pass the argument for the number of threads to the test function
  c1.wait();
}
