/*
 * ThreadMandelbrot.cpp
 */

#include "ThreadMandelbrot.h"

ThreadMandelbrot::ThreadMandelbrot(unsigned threads, unsigned depth) : Mandelbrot(threads, depth) {}

void ThreadMandelbrot::draw(CartesianRasterCanvas& can) {
  const int CH = can.getWindowHeight();   //Height of our Mandelbrot canvas
  while(myRedraw) {
    myRedraw = false;
    can.reset();
    #pragma omp parallel num_threads(myThreads)
    {
      unsigned tid = omp_get_thread_num();
      unsigned nthreads = omp_get_num_threads();
      ColorFloat tcolor = Colors::highContrastColor(tid);
      double blocksize = can.getCartHeight() / nthreads;
      double blockheight = CH / nthreads;
      long double startrow = can.getMaxY() - blocksize * tid;
      for(unsigned int k = 0; k <= blockheight && can.isOpen(); k++) {  // As long as we aren't trying to render off of the screen...
        long double row = startrow - can.getPixelHeight() * k;
        for(long double col = can.getMinX(); col <= can.getMaxX(); col += can.getPixelWidth()) {
          complex originalComplex(col, row);
          complex c(col, row);
          unsigned iterations = 0;
          while (std::abs(c) < 2.0 && iterations != myDepth) {  // Compute it until it escapes or we give up
            iterations++;
            c = c * c + originalComplex;
          }
          if(iterations == myDepth) { // If the point never escaped, draw it black
            can.drawPoint(col, row, BLACK);
          } else { // Otherwise, draw it with color based on how long it took
            float mult = iterations/(float)myDepth;
            can.drawPoint(col, row, Colors::blend(tcolor,WHITE,0.25f+0.5f*mult)*mult);
          }
          if (myRedraw) break;
        }
        can.handleIO();
        if (myRedraw) break;
      }
    }
    printf("%f seconds to draw\n", can.getTime());
    printf("%Lfx scale\n", 1/(can.getCartHeight()/2));
    while (can.isOpen() && !myRedraw) {
      can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
    }
  }
}
