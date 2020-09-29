/*
 * Julia.cpp
 */

#include "Julia.h"

Julia::Julia(unsigned threads, unsigned depth) : Mandelbrot(threads, depth) {}

void Julia::draw(Cart& can) {
  CartesianBackground * cart = can.getBackground();
  const int CH = can.getWindowHeight();   //Height of our Mandelbrot canvas
  VisualTaskQueue vq(CH);
  while(myRedraw) {
    myRedraw = false;
    can.reset();
    unsigned next = 0;
    vq.reset();
    #pragma omp parallel num_threads(myThreads)
    {
      if (omp_get_thread_num() == 0) // showLegend creates a Canvas, which should ONLY be done by thread 0
        vq.showLegend();
      #pragma omp barrier
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
          complex z(-0.8f, 0.156f);
          complex c(col, row);
          unsigned iterations = 0;
          while (std::abs(c) < 2.0 && iterations != myDepth) {  // Compute it until it escapes or we give up
            iterations++;
            c = c * c + z;
          }
          if(iterations == myDepth) { // If the point never escaped, draw it black
            cart->drawPixel(col, row, ColorInt(0,0,0,255));
          } else { // Otherwise, draw it with color based on how long it took
            float mult = iterations/(float)myDepth;
            cart->drawPixel(col, row, Colors::blend(BLACK,WHITE,0.25f+0.5f*mult)*mult);
          }
          if (!can.isOpen() || myRedraw) break;
        }
        vq.update(myNext,FINISHED);
        can.handleIO();
        if (myRedraw) break;
      }
    }
//    manhattanShading(can);
    std::cout << can.getTime() << std::endl;
    while (can.isOpen() && !myRedraw) {
      can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
      vq.sleep();
    }
  }
  vq.close();
  can.close();
}
