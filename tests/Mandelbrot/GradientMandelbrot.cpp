/*
 * GradientMandelbrot.cpp
 */

#include "GradientMandelbrot.h"

GradientMandelbrot::GradientMandelbrot(unsigned threads, unsigned depth) : Mandelbrot(threads, depth) {}

void GradientMandelbrot::draw(CartesianRasterCanvas& can) {
  while (myRedraw) {
    myRedraw = false;
    #pragma omp parallel num_threads(myThreads)
    {
      unsigned int nthreads = omp_get_num_threads();
      double blockstart = can.getCartHeight() / nthreads;
      unsigned int iterations;
      double smooth;
      for (unsigned int k = 0; k <= (can.getWindowHeight() / nthreads) && can.isOpen(); k++) {  // As long as we aren't trying to render off of the screen...
        long double row = blockstart * omp_get_thread_num() + can.getMinY() + can.getPixelHeight() * k;
        for (long double col = can.getMinX(); col <= can.getMaxX(); col += can.getPixelWidth()) {
          complex c(col, row);
          complex z(col, row);
          iterations = 0;
          while (std::abs(z) < 2.0l && iterations < myDepth) {
            iterations++;
            z = z * z + c;
          }
          if( iterations >= myDepth-1 ) { //If iterations reached myDepth
            can.drawPoint(col, row, BLACK);
          } else {
            float value = (float)iterations/myDepth;
            float mu = iterations + 1 - log( log( std::abs(z) )) / log(2);
            if( mu < 0 ) can.drawPoint(col, row, ColorHSV(0, 1.0f, 0.6f, 1.0f)); //If we have a negative adjusted iterations
            else can.drawPoint(col, row, ColorHSV((mu/(float)myDepth)*6.0f, 1.0f, 0.6f, 1.0f));
          }
          if (myRedraw) break;
        }
        can.handleIO();
        if (myRedraw) break;
      }
    }
    printf("%f seconds to draw\n", can.getTime());
    printf("%Lfx scale\n", 1/(can.getCartHeight()/2));
    while (can.isOpen() && !myRedraw)
      can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
  }
}
