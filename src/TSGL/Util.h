/*
 * Util.h contains commonly used constants and typedefs as well as some utility functions.
 */

#ifndef SRC_TSGL_UTIL_H_
#define SRC_TSGL_UTIL_H_

#include <cmath>  //To determine M_PI and is also used for math operations

namespace tsgl {

class CartesianCanvas;  //Forward declaration for typedef

//Constants to be used by TSGL classes and tests
#ifdef _WIN32
const double PI = 3.14159265358979323846;
#else
const double PI = M_PI;
#endif
const double RAD = PI / 180;     // One radian in degrees

const int FPS = 60;                // Frames per second
const float FRAME = 1.0f/FPS;      // Number of seconds between frames

/*!
 * \var typedef long double Decimal
 * \brief A type definition for a long double.
 * \note Used by: CartesianCanvas, Function, and IntegralViewer.
 */
typedef long double Decimal;  // Define the variable type to use for coordinates
typedef CartesianCanvas Cart; // Shorter name for CartesianCanvas

const int NUM_COLORS = 256, MAX_COLOR = 255;

inline bool clamp(float& f, float min, float max) {
  if (f < min) {
    f = min; return true;
  }
  if (f > max) {
    f = max; return true;
  }
  return false;
}

inline bool clamp(int& i, int min, int max) {
  if (i < min) {
    i = min; return true;
  }
  if (i > max) {
    i = max; return true;
  }
  return false;
}

}

#endif /* SRC_TSGL_UTIL_H_ */
