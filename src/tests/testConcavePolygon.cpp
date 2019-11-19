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
 * - Have four arrays of integers \b x, \b y, \b xx, and \b yy and set them to have size \b PSIZE.
 * - Create an empty array of colors of size \b PSIZE and fill it with random colors.
 * - Fill the arrays of integers, \b x and \b y with specific values (which will then be used in the while loop to draw a Concave polygon).
 * - Fill the other arrays of integers, \b xx and \b yy, with specific values.
 * - While the Canvas is open:
 *   - Sleep the internal timer of the Canvas until the Canvas is ready to draw.
 *   - Draw a Concave polygon on the Canvas and pass \b PSIZE, the arrays \b x and \b y, and the array of colors as arguments.
 *   - Draw another Concave polygon on the Canvas and pass \b PSIZE, the arrays \b x and \b y, and the array of colors as arguments.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void concavePolygonFunction(Canvas& can) {
  const int PSIZE = 64;

  int x[PSIZE], xx[PSIZE];
  int y[PSIZE], yy[PSIZE];
  ColorFloat color[PSIZE];
  for (unsigned i = 0; i < PSIZE; ++i)
    color[i] = Colors::randomColor(1.0f);

  x[0] = 100; y[0] = 100;
  x[1] = 200; y[1] = 100;
  x[2] = 200; y[2] = 200;
  x[3] = 300; y[3] = 200;
  x[4] = 300; y[4] = 100;
  x[5] = 400; y[5] = 100;
  x[6] = 400; y[6] = 400;
  x[7] = 300; y[7] = 300;
  x[8] = 250; y[8] = 400;
  x[9] = 200; y[9] = 300;
  x[10] = 100; y[10] = 400;

  for (int i = 0; i < PSIZE; ++i) {
    if (i % 2 == 0) {
      xx[i] = can.getWindowWidth() / 2 + 150 * sin((1.0f*i)/(PSIZE) * PI * 2);
      yy[i] = can.getWindowHeight() / 2 - 150 * cos((1.0f*i)/(PSIZE) * PI * 2);
    } else {
      xx[i] = can.getWindowWidth() / 2 + 300 * sin((1.0f*i)/(PSIZE) * PI * 2);
      yy[i] = can.getWindowHeight() / 2 - 300 * cos((1.0f*i)/(PSIZE) * PI * 2);
    }
  }

  ConcavePolygon * c1 = new ConcavePolygon(11, x, y, color, false);
  ConcavePolygon * c2 = new ConcavePolygon(PSIZE, xx, yy, color, true);
  can.add(c1); can.add(c2);

  while (can.isOpen()) {  // Checks to see if the window has been closed
    can.sleep();
    // note: when you call drawConcavePolygon, you MUST give it the correct size.
    // otherwise, it is always wrong and inconsistent in how it is wrong.
    can.pauseDrawing();
    c1->setCenter(can.getWindowWidth() / 2 + 450 * sin((1.0f*can.getFrameNumber() / 8)/(PSIZE) * PI * 2), can.getWindowHeight() / 2 - 450 * cos((1.0f*can.getFrameNumber() / 8)/(PSIZE) * PI * 2) );
    can.resumeDrawing();
  }

  delete c1;
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
