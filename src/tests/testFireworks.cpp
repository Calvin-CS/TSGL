/*
 * testFireworks.cpp
 *
 * Usage: ./testFireworks <width> <height> <numThreads> <numFireworks> <speed>
 */

#include <tsgl.h>
#include "Fireworks/Arc.h"

using namespace tsgl;

/*!
 * \brief Creates a bunch of fireworks.
 * \details Creates a bunch of Arcs, which explode into new arcs 
 *  \param can Canvas reference to which the fireworks will be drawn.
 *  \param threads Number of threads to use to draw the fireworks.
 *  \param numFireworks Number of fireworks to draw.
 *  \param speed How fast the fireworks move. 
 */
void fireworkFunction(Canvas& can, int threads, int numFireworks, int speed) {
  Arc** arcs = new Arc*[numFireworks];
  for (int i = 0; i < numFireworks; i++) {
    arcs[i] = new Arc(can);
  }
  ColorFloat col = can.getBackgroundColor();
  col.A = 0.04f;
  const int CWW = can.getWindowWidth(), CWH = can.getWindowHeight();
  while(can.isOpen()) {
    #pragma omp parallel num_threads(threads)
    {
    int tid = omp_get_thread_num();
    int nthreads = omp_get_num_threads();
    for (int n = 0; n < speed; ++n) {
      for (int i = tid; i < numFireworks; i += nthreads)
        arcs[i]->step();
      if (tid == 0)
        can.drawRectangle(0,0,CWW,CWH,col);
      #pragma omp barrier
    }
    can.sleep();
    }
  }
  for (int i = 0; i < numFireworks; i++) {
    delete arcs[i];
  }
  delete [] arcs;
}

int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
  int h = (argc > 2) ? atoi(argv[2]) : w;
  if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
    w = h = 960;            //If not, set the width and height to a default value
  int t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();
  int f = (argc > 4) ? atoi(argv[4]) : 50;
  int s = (argc > 5) ? atoi(argv[5]) : 10;
  Canvas c(-1, -1, w, h, "Fireworks!");
  c.setBackgroundColor(BLACK);
  c.start();
  fireworkFunction(c,t,f,s);
}
