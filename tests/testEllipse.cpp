/*
 * testEllipses.cpp
 *
 * Usage: ./testEllipses <width> <height> <numEllipses>
 */

#include <tsgl.h>
#include <vector>
#include "Util.h"

using namespace tsgl;

/*!
 * \brief Draws circles in the center of the Canvas
 */
void lineFanFunction(Canvas& can, int numEllipses) {
  can.setBackgroundColor(WHITE);
  int width = can.getWindowWidth(), height = can.getWindowHeight();
  std::vector<Ellipse*> circles;

  for(int i = numEllipses; i > 0; i--) {
    float xRadius = width/2 * i / numEllipses;
    float yRadius = height/2 * i / numEllipses;
    Ellipse* c = new Ellipse(width/2, height/2, xRadius, yRadius, Colors::highContrastColor(i));
    can.add(c); circles.push_back(c);
  }

  can.wait();

  for(int i = 0; i < circles.size(); i++) {
    delete circles[i];
  }
}

//Takes command-line arguments for the width and height of the screen as well as for the
//number of threads to use in the function
int main(int argc, char** argv) {
  int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
  int h = (argc > 2) ? atoi(argv[2]) : 0.75*w;
  if (w <= 0 || h <= 0) {    //Checked the passed width and height if they are valid
    w = 1200;
    h = 500;            //If not, set the width and height to a default value
  }
  unsigned numEllipses = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();   //Get the number of threads to use
  Canvas c(-1,-1,w,h,"Ellipses");
  c.run(lineFanFunction,numEllipses);
}
