/*
 * Nova.cpp
 */

#include "Nova.h"

Nova::Nova(unsigned threads, unsigned depth) : Mandelbrot(threads, depth) {}

void Nova::draw(CartesianRasterCanvas& can) {
  const long double R = 1.0l;
  while (myRedraw) {
    myRedraw= false;
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
          complex z(1, 0);
          smooth = exp(-std::abs(z));
          complex n, d, c1;
          complex r(1, 0);
          iterations = 0;
          while (std::abs(z) < 2.0l && iterations != myDepth) {
            iterations++;
            n = z * z * z - 1.0l;
            d = z * z * 3.0l;
            z = z + c - (R * n / d);
            smooth += exp(-std::abs(z));
          }
          smooth /= myDepth;
          if (smooth != smooth || smooth < 0)  // Check to see if smooth is NAN
            smooth = 0;
          smooth = smooth - (int)smooth;
          if (iterations == myDepth)
            can.drawPoint(col, row, BLACK);
          else
            can.drawPoint(col, row, ColorHSV((float) smooth * 6.0f, 1.0f, (float) smooth, 1.0f));
        }
        can.handleIO();
        if (myRedraw) break;
      }
    }
    manhattanShading(can);
    std::cout << "Shading done" << std::endl;
    while (can.isOpen() && !myRedraw)
      can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
  }
}
