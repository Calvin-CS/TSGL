/*
 * testCircles.cpp
 *
 * Usage: ./testCircles <width> <height> <numCircles>
 */

#include <tsgl.h>
#include <vector>
#include "Util.h"

using namespace tsgl;

/*!
 * \brief Draws circles in the center of the Canvas
 */
void lineFanFunction(Canvas& can, int numCircles) {
  can.setBackgroundColor(WHITE);
  int width = can.getWindowWidth(), height = can.getDisplayHeight();
  std::vector<Circle*> circles;

  for(int i = numCircles; i > 0; i--) {
    float radius = width/2 * i / numCircles;
    Circle* c = new Circle(width/2, height/2, radius, Colors::highContrastColor(i));
    can.add(c); circles.push_back(c);
  }

  can.wait();

  for(unsigned i = 0; i < circles.size(); i++) {
    delete circles[i];
  }
}

//Takes command-line arguments for the width and height of the screen as well as for the
//number of threads to use in the function
int main(int argc, char** argv) {
  int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
  int h = (argc > 2) ? atoi(argv[2]) : 0.75*w;
  if (w <= 0 || h <= 0) {    //Checked the passed width and height if they are valid
    w = 900;
    h = 900;            //If not, set the width and height to a default value
  }
  unsigned numCircles = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();   //Get the number of threads to use
  Canvas c(-1,-1,w,h,"Circles");
  c.run(lineFanFunction,numCircles);
}
