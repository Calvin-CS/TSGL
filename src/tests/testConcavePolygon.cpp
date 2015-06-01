/*
 * testConcavePolygon.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <cmath>
#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>

#ifdef _WIN32
const double PI = 3.1415926535;
#else
const double PI = M_PI;
#endif
const double RAD = PI / 180;  // One radian in degrees

// Some constants that get used a lot
const int NUM_COLORS = 256, MAX_COLOR = 255;

// Shared values between langton functions
enum direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

typedef CartesianCanvas Cart;
typedef std::complex<long double> complex;

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

const int IPF = 1000;  //For those functions that need it

float randfloat(int divisor = 10000) {
    return (rand() % divisor) / (float) divisor;
}

/**
 * \brief Draw concave polygons, which have one or more interior angles > 180
 * ( see http://www.mathopenref.com/polygonconcave.html )
 * \details
 * - Initialize a constant \b PSIZE
 * - Have four arrays of integers \b x, \b y, \b xx, and \b yy and set them to have size \b PSIZE
 * - Create an empty array of colors of size \b PSIZE and fill it with random colors.
 * - Fill the arrays of integers, \b x and \b y with specific values (which will then be used in the while loop to draw a concave polygon).
 * - Fill the other arrays of integers, \b xx and \b yy, with specific values.
 * - While the Canvas is open:
 *   - Sleep the internal timer of the Canvas until the Canvas is ready to draw.
 *   - Draw a concave polygon on the Canvas and pass \b PSIZE, the arrays \b x and \b y, and the array of colors as arguments.
 *   - Draw another concave polygon on the Canvas and pass \b PSIZE, the arrays \b x and \b y, and the array of colors as arguments.
 *
 * \param can, Reference to the Canvas being drawn to
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
  x[11] = 100; y[11] = 100;


//Could use #pragma omp parallel for to speed this up?
  for (int i = 0; i < PSIZE; ++i) {
    if (i % 2 == 0) {
      xx[i] = 600 + 150 * sin((1.0f*i)/(PSIZE) * PI * 2);
      yy[i] = 450 - 150 * cos((1.0f*i)/(PSIZE) * PI * 2);
    } else {
      xx[i] = 600 + 300 * sin((1.0f*i)/(PSIZE) * PI * 2);
      yy[i] = 450 - 300 * cos((1.0f*i)/(PSIZE) * PI * 2);
    }
    std::cout << i << ":" << x[i] << "," << y[i] << std::endl;
  }

  while (can.getIsOpen()) {  // Checks to see if the window has been closed
    can.sleep();
//    for (unsigned i = 0; i < PSIZE; ++i)
//      color[i] = Colors::randomColor(1.0f);
    can.drawConcavePolygon(PSIZE, x, y, color, true);
    can.drawConcavePolygon(PSIZE, xx, yy, color, true);
  }
}

int main() {
    Canvas c33(0, 0, 800, 600, "", FRAME);
    c33.setBackgroundColor(WHITE);
    c33.start();
    concavePolygonFunction(c33);
    c33.close();
}
