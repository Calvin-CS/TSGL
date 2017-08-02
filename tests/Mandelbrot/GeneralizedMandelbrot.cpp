/*
 * GeneralizedMandelbrot.cpp
 */

#include "GeneralizedMandelbrot.h"

GeneralizedMandelbrot::GeneralizedMandelbrot(unsigned threads, unsigned depth) : Mandelbrot(threads, depth) {}

void GeneralizedMandelbrot::draw(CartesianRasterCanvas& can) {
  const int CH = can.getWindowHeight();   //Height of our Mandelbrot canvas
  complex n(4, 0); //Value for z = z^n + c
  while(myRedraw) {
    myRedraw = false;
    can.reset();
    unsigned next = 0;
    #pragma omp parallel num_threads(myThreads)
    {
      int myNext;
      while(true) {  // As long as we aren't trying to render off of the screen...
        #pragma omp critical
        {
          myNext = next++;
        }
        if (myNext >= can.getWindowHeight())
          break;
        long double row = can.getMinY() + can.getPixelHeight() * myNext;
        for(long double col = can.getMinX(); col <= can.getMaxX(); col += can.getPixelWidth()) {
          complex c(col, row);
          complex z(col, row);
          unsigned iterations = 0;
          while (std::abs(z) < 2.0 && iterations != myDepth) {  // Compute it until it escapes or we give up
            iterations++;
            //Calculate z = z^n + c
            z = std::pow(z, n) + c;
          }
          if(iterations == myDepth) { // If the point never escaped, draw it black
            can.drawPoint(col, row, BLACK);
          } else { // Otherwise, draw it with color based on how long it took
            float mult = iterations/(float)myDepth;
            can.drawPoint(col, row, ColorHSV(mult*6.0f, 1.0f, 0.6f, 1.0f));
            // can.drawPoint(col, row, Colors::blend(BLACK,WHITE,0.25f+0.5f*mult)*mult);
          }
          if (!can.isOpen() || myRedraw) break;
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
