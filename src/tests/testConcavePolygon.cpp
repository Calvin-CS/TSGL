/*
 * testConcavePolygon.cpp
 *
 * Usage: ./testConcavePolygon <width> <height>
 */

#include <tsgl.h>
#include "Util.h"  //Constants

using namespace tsgl;

/**
 * \brief Draw Concave polygons, which have one or more interior angles > 180
 * \note See http://www.mathopenref.com/polygonconcave.html
 * \details
 * - Initialize a constant \b PSIZE.
 * - Have two arrays of integers, \b xx and \b yy and set them to have size \b PSIZE.
 * - Create an empty array of colors of size \b PSIZE and fill it with random colors.
 * - Fill the arrays of integers, \b xx and \b yy, with specific values.
 * - While the Canvas is open:
 *   - Sleep the internal timer of the Canvas until the Canvas is ready to draw.
 *   - Draw a Concave polygon on the Canvas and pass x-coordinate, y-coordinate, z-coordinate, \b PSIZE, the arrays \b xx and \b yy, yaw, pitch, roll, and the array of colors as arguments.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void concavePolygonFunction(Canvas& can) {
  const int PSIZE = 64;

  float xx[PSIZE];
  float yy[PSIZE];
  ColorFloat color[PSIZE];
  for (unsigned i = 0; i < PSIZE; ++i)
    color[i] = ColorFloat(float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), 1);

  color[1] = color[PSIZE-1];

  xx[0] = 0;
  yy[0] = 0;
  for (int i = 0; i < PSIZE-1; ++i) {
    if (i % 2 == 0) {
      xx[i+1] = 0 + 2.5 * sin((1.0f*i)/(PSIZE-2) * PI * 2);
      yy[i+1] = 0 - 2.5 * cos((1.0f*i)/(PSIZE-2) * PI * 2);
    } else {
      xx[i+1] = 0 + 1.5 * sin((1.0f*i)/(PSIZE-2) * PI * 2);
      yy[i+1] = 0 - 1.5 * cos((1.0f*i)/(PSIZE-2) * PI * 2);
    }
  }

  // for (int i = 0; i < PSIZE; ++i) {
  //   if (i % 2 == 0) {
  //     xx[i] = 0 + 2.5 * sin((1.0f*i)/(PSIZE) * PI * 2);
  //     yy[i] = 0 - 2.5 * cos((1.0f*i)/(PSIZE) * PI * 2);
  //   } else {
  //     xx[i] = 0 + 1.5 * sin((1.0f*i)/(PSIZE) * PI * 2);
  //     yy[i] = 0 - 1.5 * cos((1.0f*i)/(PSIZE) * PI * 2);
  //   }
  // }

  ConcavePolygon * c2 = new ConcavePolygon(0,0,0,PSIZE, xx, yy, 0,0,0,color);
  // c2->setColor(color);
  // c2->setColor(RED);
  can.add(c2);

  float floatVal = 0.0f;
  while (can.isOpen()) {  // Checks to see if the window has been closed
    can.sleep();
    // c2->setCenterX(sin(floatVal/90));
    // c2->setCenterY(sin(floatVal/90));
    // c2->setCenterZ(sin(floatVal/90));
    // c2->setYaw(floatVal);
    // c2->setPitch(floatVal);
    // c2->setRoll(floatVal);
    floatVal += 1;
  }

  delete c2;
}

//Takes command-line arguments for the width and height of the screen
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;              //If not, set the width and height to a default value
    Canvas c(-1, -1, w, h, "Concave Polygons");
    c.setBackgroundColor(WHITE);
    c.run(concavePolygonFunction);
}
