/*
 * testNova.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <complex>
#include <omp.h>
#include <tsgl.h>

using namespace tsgl;

typedef CartesianCanvas Cart;
typedef std::complex<long double> complex;

/*!
 * \brief Draws a gradient Nova set on a CartesianCanvas
 * \details Same as gradientMandelbrotFunction(), but with a nova fractal ( more time-consuming
 * to compute; see http://en.wikipedia.org/wiki/Nova_fractal ).
 * \param can, Reference to the CartesianCanvas being drawn to
 */
void novaFunction(CartesianCanvas& can, unsigned int & numberOfThreads) {
  const unsigned int DEPTH = 200;
  const long double R = 1.0l;
#pragma omp parallel num_threads(numberOfThreads)
  {
    int nthreads = omp_get_num_threads();  //Temp variable
    int tid = omp_get_thread_num();
    double BLOCKSTART = (can.getMaxY() - can.getMinY()) / nthreads;
    unsigned int iterations;
    double smooth;
    for (int k = 0; k <= (can.getWindowHeight() / nthreads) && can.getIsOpen(); k++) {  // As long as we aren't trying to render off of the screen...
      long double row = BLOCKSTART * tid + can.getMinY() + can.getPixelHeight() * k;
      for (long double col = can.getMinX(); col <= can.getMaxX(); col += can.getPixelWidth()) {
        complex c(col, row);
        complex z(1, 0);
        smooth = exp(-std::abs(z));
        complex n, d, c1;
        complex r(1, 0);
        iterations = 0;
        while (std::abs(z) < 2.0l && iterations != DEPTH) {
          iterations++;
          n = z * z * z - 1.0l;
          d = z * z * 3.0l;
          z = z + c - (R * n / d);
          smooth += exp(-std::abs(z));
        }
        for (int i = 0; i < 20; i++) {
          iterations++;
          n = z * z * z - 1.0l;
          d = z * z * 3.0l;
          z = z + c - (R * n / d);
          smooth += exp(-std::abs(z));
        }
        smooth /= DEPTH;
        if (smooth != smooth || smooth < 0)  // Check to see if smooth is NAN
          smooth = 0;
        while (smooth > 1)
          smooth -= 1;
        can.drawPoint(col, row, ColorHSV((float) smooth * 6.0f, 1.0f, (float) smooth, 1.0f));
      }
    }
  }
}

int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
  int h = (argc > 2) ? atoi(argv[2]) : 0.75*w;
  if (w <= 0 || h <= 0) {     //Checked the passed width and height if they are valid
    w = 1200;
    h = 900;                  //If not, set the width and height to a default value
  }
  Cart c17(-1, -1, w, h, -1, -0.5, 0, 0.5, "");
  unsigned t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();  //Number of threads
  c17.start();
  novaFunction(c17, t);  //Pass that as an argument
  c17.wait();
}
