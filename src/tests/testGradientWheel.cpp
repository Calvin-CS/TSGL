/*
 * testGradientWheel.cpp
 *
 * Usage: ./testGradientWheel <width> <height> <numThreads>
 */

#include <omp.h>
#include <tsgl.h>
#include "Util.h"

using namespace tsgl;

/*!
 * \brief Draws a gradient color wheel using OMP with multiple threads per processor and TSGL's colored polygons.
 * \details Same principle as colorWheelFunction(). Since colored polygons take arrays as parameters
 * to allow for arbitrary-length polygons, there are some key differences:
 * - Colors, x and y coordinates are declared within the #pragma omp block so they can be
 *   declared as an array.
 * - At the end, drawColoredPolygon() is called on a polygon with 3 vertices, with arrays for the
 *   x coordinates, y coordinates, and color.
 * \param can Reference to the Canvas being drawn to.
 */
void gradientWheelFunction(Canvas& can, int threads) {
  const int CW = can.getWindowWidth() / 2,         // Half the window's width
            CH = can.getWindowHeight() / 2;        // Half the window's height
  const float RADIUS = (CH < CW ? CH : CW) * .95,  // Radius of wheel
              ARCLENGTH = 2 * PI / NUM_COLORS;     // Gap between wedges
  #pragma omp parallel num_threads(threads)
  {
    threads = omp_get_num_threads();               // Actual number of threads
    int tid = omp_get_thread_num();                // Thread ID
    int delta = (NUM_COLORS / threads);            // Distance between threads to compute
    float shading = 1 - (float)tid / threads;      // Shading based on thread ID
    ColorFloat color[3];                           // RGB color to build
    int start, end, xx[3], yy[3];                  // Setup the arrays of values for vertices
    while (can.getIsOpen()) {
      can.sleep();
      start = (NUM_COLORS - (can.getReps() % NUM_COLORS) + tid*delta) % NUM_COLORS; // Starting hue of the segment
      end = ((start+delta) % NUM_COLORS);

      color[0] = ColorHSV(start / (float)NUM_COLORS * 6, 0.0f, shading, 1.0f);
      color[1] = ColorHSV(start / (float)NUM_COLORS * 6, 1.0f, shading, 1.0f);
      color[2] = ColorHSV(end   / (float)NUM_COLORS * 6, 1.0f, shading, 1.0f);

      xx[0] = CW; yy[0] = CH;                           // Set first vertex to center of screen
      xx[1] = CW + RADIUS * sin(ARCLENGTH * start);     // Add the next two vertices around the circle
      yy[1] = CH + RADIUS * cos(ARCLENGTH * start);
      xx[2] = CW + RADIUS * sin(ARCLENGTH * (start + 1));
      yy[2] = CH + RADIUS * cos(ARCLENGTH * (start + 1));

      can.drawColoredPolygon(3, xx, yy, color);
    }
  }
}

//Takes command line arguments for the width and height of the window
int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
  int h = (argc > 2) ? atoi(argv[2]) : w;
  if (w <= 0 || h <= 0)     // Checked the passed width and height if they are valid
    w = h = 960;            // If not, set the width and height to a default value
  int t = (argc > 3) ? atoi(argv[3]) : 256;
  Canvas c(-1, -1, w, h, "Gradient Color Wheel");
  c.setBackgroundColor(BLACK);
  c.run(gradientWheelFunction,t);
}
