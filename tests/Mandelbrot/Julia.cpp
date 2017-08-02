/*
 * Julia.cpp
 */

#include "Julia.h"

Julia::Julia(unsigned threads, unsigned depth) : Mandelbrot(threads, depth) {}

void Julia::draw(CartesianRasterCanvas& can) {
  const int CH = can.getWindowHeight();   //Height of our Mandelbrot canvas
  VisualTaskQueue vq(CH);
  while(myRedraw) {
    myRedraw = false;
    can.reset();
    int next = 0;
    vq.reset();
    #pragma omp parallel num_threads(myThreads)
    {
      vq.showLegend();
      int myNext;
      while(true) {  // As long as we aren't trying to render off of the screen...
        #pragma omp critical
        {
          myNext = next++;
        }
        if (myNext >= can.getWindowHeight())
          break;
        vq.update(myNext,RUNNING);
        long double row = can.getMinY() + can.getPixelHeight() * myNext;
        for(long double col = can.getMinX(); col <= can.getMaxX(); col += can.getPixelWidth()) {

          //Uncomment one line to choose values, or make your own
          complex c(-0.8f, 0.156f); complex n(2, 0);
          // complex c(-0.4, 0.6); complex n(2, 0);
          // complex c(0.285, 0); complex n(2, 0);
          // complex c(0.4); complex n(3, 0);
          // complex c(-0.1, 0.651); complex n(2, 0);
          // complex c(-0.75, 0.11); complex n(2, 0);

          complex z(col, row);
          unsigned iterations = 0;
          while (std::abs(z) < 2.0 && iterations != myDepth) {  // Compute it until it escapes or we give up
            iterations++;
            z = std::pow(z, n) + c;
          }
          if(iterations == myDepth) { // If the point never escaped, draw it black
            can.drawPoint(col, row, BLACK);
          } else { // Otherwise, draw it with color based on how long it took
            float mult = iterations/(float)myDepth;
            can.drawPoint(col, row, ColorHSV(mult*6.0f, 1.0f, 0.6f, 1.0f));
          }
          if (!can.isOpen() || myRedraw) break;
        }
        vq.update(myNext,FINISHED);
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
  vq.close();
}
