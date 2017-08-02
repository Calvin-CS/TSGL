/*
 * FastMandelbrot.cpp
 */

#include "FastMandelbrot.h"

FastMandelbrot::FastMandelbrot(unsigned threads, unsigned depth) : Mandelbrot(threads, depth) { }

void FastMandelbrot::draw(CartesianRasterCanvas& can) {
  const int CH = can.getWindowHeight();   //Height of our FastMandelbrot canvas
  while(myRedraw) {
    myRedraw = false;
    can.reset();
    int next = 0;
    Decimal maxX = can.getMaxX(); Decimal minX = can.getMinX();
    #pragma omp parallel num_threads(myThreads)
    {
      while(true) {
        int myNext;
        #pragma omp critical
        {
          myNext = next++;
        }
        if (myNext >= can.getWindowHeight())
          break;
        Decimal row = can.getMaxY() - myNext*can.getPixelHeight();
        for(Decimal col = minX; col <= maxX; col += can.getPixelWidth()) {
          complex originalComplex(col, row);
          complex z(col, row);
          unsigned iterations = 0;
          while (std::abs(z) < 2.0 && iterations <= myDepth) {  // Compute it until it escapes or we give up
            iterations++;
            z = z * z + originalComplex;
          }
          if(iterations >= myDepth) { // If the point never escaped, draw it black
            can.drawPoint(col, row, BLACK);
          } else { // Otherwise, draw it with color based on how long it took
            can.drawPoint(col, row, ColorHSV(iterations*6.0f/myDepth, 1.0f, 0.6f, 1.0f));
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
