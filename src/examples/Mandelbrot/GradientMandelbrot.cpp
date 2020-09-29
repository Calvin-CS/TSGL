/*
 * GradientMandelbrot.cpp
 */

#include "GradientMandelbrot.h"

GradientMandelbrot::GradientMandelbrot(unsigned threads, unsigned depth) : Mandelbrot(threads, depth) {}

void GradientMandelbrot::draw(Cart& can) {
  CartesianBackground * cart = can.getBackground();
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
          smooth = exp(-std::abs(z));
          iterations = 0;
          while (std::abs(z) < 2.0l && iterations != myDepth) {
            iterations++;
            z = z * z + c;
            smooth += exp(-std::abs(z));
          }
          smooth /= (myDepth + 1);
          float value = (float)iterations/myDepth;
          if (row < cart->getMaxY())
            cart->drawPixel(col, row, ColorHSV((float)smooth * 6.0f, 1.0f, value, 1.0f));
          if (myRedraw) break;
        }
        can.handleIO();
        if (myRedraw) break;
      }
    }
    while (can.isOpen() && !myRedraw)
      can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
  }
}

